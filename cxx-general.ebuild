# Copyright 1999-2005 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils

DESCRIPTION="A collection of C++ wrappers and utilities."
HOMEPAGE="http://cxx-general.lepertheory.net/"
SRC_URI="http://cxx-general.lepertheory.net/${P}.tar.bz2"

LICENSE="NOTRELEASED"

SLOT="0"

KEYWORDS="~x86"

IUSE=""

DEPEND="                        \
  >=dev-python/sconsgeneral-0.3 \
  !>=dev-python/sconsgeneral-1  \
"

src_compile() {
	
	# Compile.
	scons PREFIX=/usr || die
	
}

src_install() {
	
	# Get the major/minor versions.
	VER_MAJOR=$(cat ${S}/config/LIB_VER_MAJOR)
	VER_MINOR=$(cat ${S}/config/LIB_VER_MINOR)
	
	# Install.
	mkdir -p ${D}usr
	scons PREFIX=/usr DESTDIR=${D}usr install || die
	dosym libcxx-general.so.${VER_MAJOR}.${VER_MINOR} /usr/lib/libcxx-general.so
	dosym libcxx-general.so.${VER_MAJOR}.${VER_MINOR} /usr/lib/libcxx-general.so.${VER_MAJOR}
	
}
