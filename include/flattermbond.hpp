#ifndef FLAT_TERM_BOND_HPP
#define FLAT_TERM_BOND_HPP

#include "basebond.hpp"

namespace BondLibrary {
class FlatTermBond : public BaseBond {
public:
    FlatTermBond(
        double face_value, 
        double coupon, 
        const Date maturity_date,
        const Date issue_date, 
        const CashFlowsPy& cashflows, 
        Date settlement_date,
        const DayCountConvention
    );
    double cleanPrice(const double rate, const Date date) const;
    double dirtyPrice(const double rate, const Date date) const;
    double dirtyPriceFromCleanPrice(const double market_price, const Date date) const;
    double duration(const double rate, const Date date) const override;
};
}

#endif
