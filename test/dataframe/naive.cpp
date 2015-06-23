#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <valarray>

class formula;
struct slice { static constexpr int all = 0; };

template <typename Dataframe>
auto naive_bayes(const formula& formula, const Dataframe& data)
{
    std::unordered_map<std::string,
        std::tuple<std::valarray<double> /* μ */, std::valarray<double> /* σ² */>
    > model;

    auto r = apply(formula, data);
    auto response = std::move(r.first);
    auto terms = std::move(r.second);

    for (auto& col : terms.columns())
        col.template coerce_to<double>();
    response[slice::all][0].template coerce_to<std::string>();

    const auto& predictive = response[slice::all][0].template interpret_as<std::string>();
    const auto& features = terms.template interpret_as<double>();

    auto classes = unique(predictive);

    for (auto& cl : classes) {
        auto ix = predictive == cl;
        model[cl] = std::make_tuple(
            colapply(features[ix], [](const auto& x) { return mean(x); }),
            colapply(features[ix], [](const auto& x) { return var(x); })
        );
    }

    return model;
}

TEST_CASE("Naïve Bayes", "[naive]") {
    // auto iris = dataframe::read("iris.csv");
    // auto model = naive_bayes(formula::response<std::string>("Species") = formula::remaining<double>, iris);
}
