# Maintainer: Martin Sigloch <pkgbuild@f0i.de>

pkgname=gringo-timetracking
pkgver=1.0.0
pkgrel=1
pkgdesc='Simple timetracking tool'
arch=('i686' 'x86_64')
url='https://github.com/schickst/TimeTracking'
license=('GPL')
depends=('qt5-base')
makedepends=('git' 'qtchooser')
options=('docs')
source=("git+https://github.com/schickst/TimeTracking")
md5sums=('SKIP') 


build() {
  [[ -d build ]] && rm -r build
  mkdir build && cd build

  chmod -x ../TimeTracking/gringo-timetracking
  LLVM_INSTALL_DIR=/usr qmake -r ../TimeTracking/Gringo.pro
  make
}

package() {
  cd build

  make INSTALL_ROOT="${pkgdir}/usr/" install
  chmod +x "${pkgdir}/usr/bin/gringo-timetracking"
}
