#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dataframe/detail/storage.hpp"

TEST_CASE("coercion", "[coercion]")
{
    using namespace jules;
    using namespace jules::dataframe_detail;

    using storage_ptr = std::unique_ptr<storage_eraser<default_coercion_rules>>;

    storage_ptr istorage{new storage<int, default_coercion_rules>};
    storage_ptr dstorage{new storage<double, default_coercion_rules>};

    REQUIRE(istorage->elements_type() == typeid(int));
    REQUIRE(dstorage->elements_type() == typeid(double));
    REQUIRE(dstorage->elements_type() != typeid(int));
    REQUIRE(istorage->elements_type() != typeid(double));

    REQUIRE(istorage->can_coerce_to<int>());
    REQUIRE(!istorage->can_coerce_to<typename default_coercion_rules::numeric_t>());
    REQUIRE(!istorage->can_coerce_to<unsigned int>());

    REQUIRE(!dstorage->can_coerce_to<int>());
    REQUIRE(dstorage->can_coerce_to<typename default_coercion_rules::numeric_t>());
    REQUIRE(dstorage->can_coerce_to<typename default_coercion_rules::string_t>());
}
