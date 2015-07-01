#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <valarray>

template <typename Dataframe> class GaussianNaiveBayes
{
  public:
    using dataframe_t = Dataframe;
    using column_t = typename dataframe_t::column_t;

    GaussianNaiveBayes(const formula& f, const Dataframe& data)
    {
        auto response = f.response(data);
        auto terms = f.terms(data);

        for (auto& col : terms.columns())
            col.template coerce_to<double>();
        response.template coerce_to<std::string>();

        const auto& predictive = response.template as_array<std::string>();
        const auto& features = terms.template as_array<double>();

        init(predictive, features);
    }

  private:
    void init(const vector<std::string>& predictive, const matrix<double>& features)
    {
        auto classes = unique(predictive);

        for (auto& cl : classes) {
            auto ix = predictive == cl;

            mu.cbind(column_t{cl, colapply(features[ix], [](const auto& x) { return mean(x); })});
            sigma.cbind(column_t{cl, colapply(features[ix], [](const auto& x) { return sd(x); })});
        }
    }

    Dataframe mu, sigma;
};

TEST_CASE("Naïve Bayes", "[naive]")
{
    // auto iris = dataframe::read("iris.csv");
    // auto model = naive_bayes(formula::response<std::string>("Species") = formula::remaining<double>, iris);
}
