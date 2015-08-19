#ifndef JULES_ARRAY_VIEW_H
#define JULES_ARRAY_VIEW_H

namespace jules
{

template <typename C> class array_view
{
  private:
      using size_type = typename C::size_type;

  public:
    array_view(C& container) : container_(container) {}

    auto& operator[](size_type i) { return container_[i]; }
    const auto& operator[](size_type i) const { return container_[i]; }

  private:
    C& container_;
};



} // namespace jules

#endif // JULES_ARRAY_VIEW_H
