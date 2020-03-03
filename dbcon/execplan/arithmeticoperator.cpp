/* Copyright (C) 2014 InfiniDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

/***********************************************************************
*   $Id: arithmeticoperator.cpp 9210 2013-01-21 14:10:42Z rdempsey $
*
*
***********************************************************************/
#include <iostream>

#include "bytestream.h"
#include "arithmeticoperator.h"
#include "objectreader.h"

using namespace std;

namespace
{

/**@brief util struct for converting string to lower case */
struct to_lower
{
    char operator() (char c) const
    {
        return tolower(c);
    }
};

}

namespace execplan
{

/**
 * Constructors/Destructors
 */
ArithmeticOperator::ArithmeticOperator() : Operator()
{
}

ArithmeticOperator::ArithmeticOperator(const string& operatorName): Operator(operatorName)
{
}

ArithmeticOperator::ArithmeticOperator(const ArithmeticOperator& rhs):
    Operator(rhs),
    fTimeZone(rhs.timeZone())
{
}

ArithmeticOperator:: ~ArithmeticOperator()
{
}

/**
 * Operations
 */

/**
 * friend function
 */
ostream& operator<<(ostream& output, const ArithmeticOperator& rhs)
{
    output << rhs.toString();
    output << "opType=" << rhs.operationType().colDataType << endl;
    return output;
}

/**
 * The serialization interface
 */
void ArithmeticOperator::serialize(messageqcpp::ByteStream& b) const
{
    b << (ObjectReader::id_t) ObjectReader::ARITHMETICOPERATOR;
    b << fTimeZone;
    Operator::serialize(b);
}

void ArithmeticOperator::unserialize(messageqcpp::ByteStream& b)
{
    ObjectReader::checkType(b, ObjectReader::ARITHMETICOPERATOR);
    b >> fTimeZone;
    Operator::unserialize(b);
}

bool ArithmeticOperator::operator==(const ArithmeticOperator& t) const
{
    if (data() != t.data())
        return false;

    if (timeZone() != t.timeZone())
        return false;

    return true;
}

bool ArithmeticOperator::operator==(const TreeNode* t) const
{
    const ArithmeticOperator* o;

    o = dynamic_cast<const ArithmeticOperator*>(t);

    if (o == NULL)
        return false;

    return *this == *o;
}

bool ArithmeticOperator::operator!=(const ArithmeticOperator& t) const
{
    return (!(*this == t));
}

bool ArithmeticOperator::operator!=(const TreeNode* t) const
{
    return (!(*this == t));
}

void ArithmeticOperator::adjustResultType(const CalpontSystemCatalog::ColType& m)
{
    if (m.colDataType != CalpontSystemCatalog::DECIMAL)
    {
        fResultType = m;
    }
    else
    {
        CalpontSystemCatalog::ColType n;
        n.colDataType = CalpontSystemCatalog::LONGDOUBLE;
        n.scale = m.scale; // @bug5736, save the original decimal scale
        n.precision = -1;  // @bug5736, indicate this double is for decimal math
        n.colWidth = sizeof(long double);
        fResultType = n;
    }
}

}  // namespace
