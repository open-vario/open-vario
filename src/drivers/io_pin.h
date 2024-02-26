/*
 * Copyright (c) 2023 open-vario
 * SPDX-License-Identifier: MIT
 */

#ifndef OV_IO_PIN_H
#define OV_IO_PIN_H

namespace ov
{
namespace io
{

/** @brief Pin level */
typedef bool level;
/** @brief High level */
static const level HIGH = true;
/** @brief Low level */
static const level LOW = false;

} // namespace io
} // namespace ov

#endif // OV_IO_PIN_H
