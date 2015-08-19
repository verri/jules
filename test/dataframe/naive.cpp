#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include <dataframe/dataframe.hpp>
#include <formula/formula.hpp>

using namespace jules;

class GaussianNaiveBayes
{
  public:
    GaussianNaiveBayes(const formula& f, const dataframe& data)
    {
        auto response = f.response(data);
        auto terms = f.terms(data);

        features = terms.colnames();

        auto predictive = make_view<std::string>(response); // predictive is array_view of string
        auto features = make_view<double>(terms); // features is array_view of array_view of double

        classes = unique(predictive);

        auto m = classes.size();
        mu.resize(m);
        sigma.resize(m);
        priori.resize(m);

        auto n = predictive.size();

        std::size_t i = 0;
        for (auto& cl : classes) {
            auto ix = predictive == cl;

            mu[i] = apply(features[ix], [](const auto& x) { return mean(x); });
            sigma[i] = apply(features[ix], [](const auto& x) { return sd(x); });
            priori[i] = count(ix) / n;

            ++i;
        }
    }

    std::string classify(const vector<double>& sample) {
        if (sample.size() != nfeatures_)
            throw std::runtime_error{"invalid sample"};

        auto likelihood = ...;

        auto posteriori = likelihood * priori;

        return classes[which_max(posteriori)];
    }


  private:
    vector<std::string> features;
    vector<std::string> classes;

    vector<vector<double>> mu;
    vector<vector<double>> sigma;
    vector<double> priori;
};

TEST_CASE("Naïve Bayes", "[naive]")
{
    // auto iris = dataframe::read("iris.csv");
    // auto model = naive_bayes(formula::response<std::string>("Species") = formula::remaining<double>, iris);
}
