#ifndef JULES_ARRAY_DETAIL_ARRAY_DECL_H
#define JULES_ARRAY_DETAIL_ARRAY_DECL_H

#include "array/detail/common.hpp"

#include <array>
#include <utility>
#include <vector>

namespace jules
{
namespace detail
{
struct trivial_tag {
};
struct non_trivial_tag {
};

template <typename T, typename = void> struct trivial_dispatch_helper {
    using type = non_trivial_tag;
};

template <typename T> struct trivial_dispatch_helper<T, std::enable_if_t<std::is_trivial<T>::value>> {
    using type = trivial_tag;
};

static_assert(std::is_same<typename trivial_dispatch_helper<double>::type, trivial_tag>::value, "");
static_assert(std::is_same<typename trivial_dispatch_helper<std::string>::type, non_trivial_tag>::value, "");

template <typename T> auto trivial_dispatch() { return typename trivial_dispatch_helper<T>::type{}; }

template <typename T, std::size_t N> class base_ndarray : public ref_ndarray<T, N>
{
    template <typename U, std::size_t M> friend class base_ndarray;

  public:
    using storage_t = std::aligned_storage_t<sizeof(T), alignof(T)>;

    base_ndarray();
    ~base_ndarray();

    template <typename... Dims, typename = all_size_enabler<N, Dims...>> explicit base_ndarray(Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T& value, Dims... dims);

    template <typename... Dims, typename = all_size_enabler<N, Dims...>>
    base_ndarray(const T* data, Dims... dims);

    base_ndarray(const base_ndarray& source);
    base_ndarray(base_ndarray&& source);

    template <typename U> base_ndarray(const base_ndarray<U, N>& source);
    template <typename U> base_ndarray(const ref_ndarray<U, N>& source);

    base_ndarray& operator=(const base_ndarray& source);
    base_ndarray& operator=(base_ndarray&& source);

    template <typename U> base_ndarray& operator=(const base_ndarray<U, N>& source);
    template <typename U> base_ndarray& operator=(const ref_ndarray<U, N>& source);
    // TODO: template <typename U> base_ndarray& operator=(const indirect_ndarray<U, N>& source);

    T* data() { return this->data_; }
    const T* data() const { return this->data_; }

    T* data_begin() { return this->data_; }
    T* data_end() { return this->data_ + this->size(); }

    const T* data_begin() const { return this->data_; }
    const T* data_end() const { return this->data_ + this->size(); }

  private:
    static T* allocate(std::size_t size) { return reinterpret_cast<T*>(new storage_t[size]); }
    static void deallocate(T* data, std::size_t) { delete[] reinterpret_cast<storage_t*>(data); }

    static void create(trivial_tag, T* data, std::size_t size);
    template <typename... Args> static void create(trivial_tag, T* data, std::size_t size, Args&&... args);
    template <typename... Args>
    static void create(non_trivial_tag, T* data, std::size_t size, Args&&... args);

    template <typename It> static void create(trivial_tag, T* to, It from, std::size_t size);
    static void create(trivial_tag, T* to, T* from, std::size_t size);

    template <typename It> static void create(non_trivial_tag, T* to, It from, std::size_t size);

    static void destroy(trivial_tag, T* data, std::size_t size);
    static void destroy(non_trivial_tag, T* data, std::size_t size);

    void clear();
};

template <typename T> class base_ndarray<T, 0>
{
  public:
    using value_type = T;

    base_ndarray() = default;

    base_ndarray(const T& data) : data_{data} {}
    base_ndarray(T&& data) : data_{std::move(data)} {}

    ~base_ndarray() = default;

    base_ndarray(const base_ndarray& source) = default;
    base_ndarray(base_ndarray&& source) = default;

    base_ndarray& operator=(const base_ndarray& source) = default;
    base_ndarray& operator=(base_ndarray&& source) = default;

    base_ndarray& operator=(const T& source) { data_ = source; }
    base_ndarray& operator=(T&& source) { data_ = std::move(source); }

    operator T&() { return data_; }
    operator const T&() const { return data_; }

    constexpr std::size_t size() const { return 0; }

    T* data() { return &data_; }
    const T* data() const { return &data_; }

  private:
    T data_;
};

} // namespace detail
} // namespace jules

#endif // JULES_ARRAY_DETAIL_ARRAY_DECL_H
