#include "catch.hpp"

#include <dataframe/dataframe.hpp>
#include <formula/formula.hpp>

#include <fstream>

using namespace jules;

class gaussian_naive_bayes
{
  public:
    gaussian_naive_bayes(const expr<>& response, const expr_list& terms, const dataframe& data)
    {
        // auto response_column = data.select(response);
        // auto terms_dataframe = data.select(terms);

        // static_assert(std::is_same<decltype(response_column), column>);
        // static_assert(std::is_same<decltype(terms_dataframe), dataframe>);

        // auto response = f.lhs(data).col(1); // response is a column
        // auto terms = f.rhs(data);           // terms is a dataframe

        // features = terms.colnames();

        // auto predictive = make_view<std::string>(response); // predictive is an array_view of string
        // auto features = make_view<all<double>>(terms); // features is an array_view of array_view of double

        // classes = unique(predictive);

        // mu.resize(classes.size());
        // sigma2.resize(classes.size());
        // priori.resize(classes.size());

        // std::size_t i = 0;
        // for (auto& cl : classes) {
        //     auto ix = predictive == cl;

        //     mu[i] = apply(features[ix], [](const auto& x) { return mean(x); });
        //     sigma2[i] = apply(features[ix], [](const auto& x) { return var(x); });
        //     priori[i] = count(ix) / ix.size();

        //     ++i;
        // }
    }

    // std::string classify(const vector<double>& sample)
    // {
    //     // if (sample.size() != nfeatures_)
    //     //     throw std::runtime_error{"invalid sample"};

    //     // vector<double> log_posteriori(classes.size());

    //     // for (auto i : range(classes.size())) {
    //     //     auto likelihood = exp(-pow(sample - mu) / (2 * sigma2)) / sqrt(2 * k::pi * sigma2);
    //     //     log_posteriori[i] = sum(log(likelihood)) + log(priori[i]);
    //     // }

    //     // return classes[which_max(posteriori)];
    // }

  private:
    // vector<std::string> features;
    // vector<std::string> classes;

    // vector<vector<double>> mu;
    // vector<vector<double>> sigma2;
    // vector<double> priori;
};

TEST_CASE("Naïve Bayes", "[naive]")
{
    std::ifstream file{"data/iris.csv"};
    const auto iris = dataframe::read(file);

    // Leave one out
    // experiment<std::size_t, bool> loo(0, [](auto acc, auto v) { return acc + (v ? 0 : 1); });
    // // Settings
    // loo.threads(8);

    // auto indexes = range(0, iris.nrow());

    // auto error = loo.expand(indexes)([&iris](auto i) {
    //     auto model = gaussian_naive_bayes{"Species" = ~remaining<double>{}, iris[-i]};
    //     return model.classify(iris[i][all_except("Species")]) == iris.at<std::string>(i, "Species");
    // });

    // std::cout << "the classifier misclassified " << error << " of " << iris.nrow() << " samples."
    //           << std::endl;
}
