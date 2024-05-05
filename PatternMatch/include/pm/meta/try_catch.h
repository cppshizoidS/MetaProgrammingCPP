#ifndef TRY_CATCH_H
#define TRY_CATCH_H

#include "./dispatch.h"

namespace pm {

namespace meta {

/**
 * @brief Тип-метка для операции try.
 *
 * Этот тип-метка используется для определения операции try.
 */
using try_t = dispatch<1>;

/**
 * @brief Тип-метка для операции catch.
 *
 * Этот тип-метка используется для определения операции catch.
 */
using catch_t = dispatch<0>;

}  // namespace meta

}  // namespace pm

#endif
