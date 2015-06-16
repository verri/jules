#ifndef JULES_DATAFRAME_DETAIL_STORAGE_H
#define JULES_DATAFRAME_DETAIL_STORAGE_H

#include <vector>

namespace jules {
namespace dataframe_detail {

class storage_eraser
{
    virtual ~storage_eraser() = 0;
};

template<typename T>
class storage : public storage_eraser, public std::vector<T>
{
private:
    using vector = std::vector<T>;

public:
    using vector::vector;
};

} // namespace dataframe_detail
} // namespace jules


#endif // JULES_DATAFRAME_DETAIL_STORAGE_H
