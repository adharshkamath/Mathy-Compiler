#ifndef BOUNDS_H
#define BOUNDS_H

#include <string>
#include <variant>

namespace mathy {
    class ForAll;

    class SigmaProd;

    class Bound {
    public:
        std::variant<int, std::string> lower, upper;
        std::string identifier;

        Bound(std::variant<std::string, double, int> low, std::string &lower_comp, const std::string &id,
              std::string &higher_comp, std::variant<std::string, double, int> high) {
            this->identifier = id;
            if (lower_comp.compare("<")) {
                if (auto lowp = std::get_if<std::string>(&low)) {
                    std::string &lv = *lowp;
                    lv.append("+1");
                    this->lower = lv;
                } else {
                    auto lowi = std::get_if<int>(&low);
                    int &lpv = *lowi;
                    this->lower = lpv + 1;
                }
            } else {
                if (auto lowp = std::get_if<std::string>(&low)) {
                    this->lower = *lowp;
                } else {
                    auto lowi = std::get_if<int>(&low);
                    int &lpv = *lowi;
                    this->lower = lpv;
                }
            }
            if (higher_comp.compare("<")) {
                if (auto highp = std::get_if<std::string>(&high)) {
                    std::string &hp = *highp;
                    hp.append("-1");
                    this->upper = hp;
                } else {
                    auto highi = std::get_if<int>(&low);
                    int &hpv = *highi;
                    this->upper = hpv - 1;
                }
            } else {
                if (auto highp = std::get_if<std::string>(&high)) {
                    std::string &hp = *highp;
                    this->upper = hp;
                } else {
                    auto highi = std::get_if<int>(&low);
                    int &hpv = *highi;
                    this->upper = hpv;
                }
            }
        }

        Bound() : lower(0), upper(10000), identifier("t") {
        }
    };
} // namespace mathy

#endif