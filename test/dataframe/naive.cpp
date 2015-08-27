#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <dataframe/dataframe.hpp>
#include <formula/formula.hpp>

using namespace jules;

class gaussian_naive_bayes
{
  public:
    gaussian_naive_bayes(const expr& response, const expr_list& terms, const dataframe& data)
    {
        auto response = f.lhs(data).col(1); // response is a column
        auto terms = f.rhs(data);           // terms is a dataframe

        features = terms.colnames();

        auto predictive = make_view<std::string>(response); // predictive is an array_view of string
        auto features = make_view<all<double>>(terms); // features is an array_view of array_view of double

        classes = unique(predictive);

        mu.resize(classes.size());
        sigma2.resize(classes.size());
        priori.resize(classes.size());

        std::size_t i = 0;
        for (auto& cl : classes) {
            auto ix = predictive == cl;

            mu[i] = apply(features[ix], [](const auto& x) { return mean(x); });
            sigma2[i] = apply(features[ix], [](const auto& x) { return var(x); });
            priori[i] = count(ix) / ix.size();

            ++i;
        }
    }

    std::string classify(const vector<double>& sample)
    {
        if (sample.size() != nfeatures_)
            throw std::runtime_error{"invalid sample"};

        vector<double> log_posteriori(classes.size());

        for (auto i : range(classes.size())) {
            auto likelihood = exp(-pow(sample - mu) / (2 * sigma2)) / sqrt(2 * k::pi * sigma2);
            log_posteriori[i] = sum(log(likelihood)) + log(priori[i]);
        }

        return classes[which_max(posteriori)];
    }

  private:
    vector<std::string> features;
    vector<std::string> classes;

    vector<vector<double>> mu;
    vector<vector<double>> sigma2;
    vector<double> priori;
};

TEST_CASE("Naïve Bayes", "[naive]")
{
    const auto iris = dataframe::read("iris.csv");

    // Leave one out
    experiment<std::size_t, bool> loo{[](auto acc, auto v) { return acc + (v ? 0 : 1); }};
    loo.threads(8);

    auto indexes = range(iris.nrow());

    auto error = loo.foreach (indexes)([&iris, formula](auto i) {
        auto model = gaussian_naive_bayes{"Species" = ~remaining<double>{}, iris[-i]};
        return model.classify(iris[i][all_except("Species")]) == iris[i]["Species"];
    });

    std::cout << "the classifier misclassified " << error << " of " << iris.nrow() << " samples."
              << std::endl;
}
