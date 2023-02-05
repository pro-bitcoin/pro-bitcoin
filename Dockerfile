FROM ubuntu:22.10 AS build
ARG NPROC 1
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    git libtool autotools-dev automake pkg-config bsdmainutils curl ca-certificates procps bison \
    gpg software-properties-common libxml2-utils cmake zlib1g-dev prometheus-cpp-dev libboost-dev libsqlite3-dev \
    libevent-dev bsdmainutils

WORKDIR /app
COPY . .

RUN ./autogen.sh

RUN  ./configure \
    --prefix=/usr \
    --sbindir=/usr/bin \
    --libexecdir=/usr/lib/bitcoin \
    --sysconfdir=/etc \
    --sharedstatedir=/usr/share/bitcoin \
    --localstatedir=/var/lib/bitcoin \
    --enable-hardening \
    --with-gui=no \
    --disable-wallet \
    --disable-tests \
    --disable-gui-tests \
    --disable-bench \
    --disable-libs \
    --without-miniupnpc \
    --disable-util-cli \
    --disable-util-tx \
    --disable-util-util \
    --enable-usdt \
    --with-gnu-ld \
    --disable-man
    
RUN make -j ${NPROC:-1}

FROM ubuntu:22.10
RUN apt-get update && apt-get install -y \
    curl wget bash ca-certificates \
    zlib1g prometheus-cpp-dev libevent-dev

COPY --from=build /app/src/bitcoind /usr/bin/pro-bitcoind
RUN mkdir -p /var/lib/bitcoind /etc/bitcoin
COPY --from=build /app/contrib/bitcoin.conf /etc/bitcoin/bitcoin.conf

CMD ["/usr/bin/pro-bitcoind", "-chain=main", "-conf=/etc/bitcoin/bitcoin.conf", "-datadir=/var/lib/bitcoind" ]

