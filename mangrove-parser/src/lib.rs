// SPDX-License-Identifier: BSD-3-Clause
// SPDX-FileCopyrightText: 2026 Mangrove Programming Language
// SPDX-FileContributor: Written by Rachel Mant <git@dragonmux.network>

use pest_derive::Parser;

#[derive(Parser)]
#[grammar = "mangrove.pest"]
pub struct MangroveParser;
