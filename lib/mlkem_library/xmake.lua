-- Copyright CHERIoT Contributors.
-- SPDX-License-Identifier: MIT

library("mlkem_library")
	add_deps("randombytes")
	add_includedirs("../../include", {public=true})
	add_includedirs("../../third_party/mlkem-native/mlkem")
	add_files(
		"../../third_party/mlkem-native/mlkem/mlkem_native.c")
