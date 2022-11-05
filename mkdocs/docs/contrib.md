---
title: Getting Started
description: TBD
---


# Prerequisites

  Below are some pointers/concepts to get you started on your bitcoin-core walk-through journey,
  For installation of a prometheus enabled bitcoin node see [install](/install) guide,

## Core Concepts

  Understanding of basic blockchain semantics, [transactions](), [hashes]() and [UTXO]() model is most helpful when reviewing code and flow.
  One concept that really helped me is [immutable](https://en.wikipedia.org/wiki/Immutable_object) coins. Coins are destroyed as they are spent.
  Suppose I hold 1 BTC and want to give you 0.5 BTC. In a transaction, I **destroy** my 1 BTC, give you 0.5 BTC and send the rest (0.5) back to me (called a [change]() address)
  Unlike a traditional banking model in which I subtract my deposit account and increase (or credit) your account by 0.05 BTC.
  Destroying coins is an easier model for algorithms which reduces complexity. The [double spending]() of coins is much easier to detect as well as maintaining consensus across the network

  Understanding encryption is also **not** necessary and out of scope.

  In all, these images should not be a foreign concept:

<div>
<img src="/assets/images/block.png" width="812px" height="200px"/>
</div>
<div>
<img src="/assets/images/transaction.png" width="400px" height="200px"/>
</div>

---


## RTFD

  The [documentation](https://github.com/bitcoin/bitcoin/tree/master/doc) for bitcoin core is rather extensive and very informative.
  Anyone interested in participating in bitcoin development should take the time to review
  [developer-notes](https://github.com/bitcoin/bitcoin/blob/master/doc/developer-notes.md){:target="_blank"} and [file layout](https://github.com/bitcoin/bitcoin/blob/master/doc/files.md) are good places to start your journey.

---

## CPP

  You need not be an expert in CPP language to be effective. There are comprehensive tests done in [python](https://github.com/bitcoin/bitcoin/tree/master/test/functional)
  as well as shell [scripts](https://github.com/bitcoin/bitcoin/tree/master/ci/test) for [CI](https://github.com/bitcoin/bitcoin/tree/master/ci)
  It may seem overwhelming at first. Bitcoin core contains a GUI, wallet and mining implementations of which all are out of scope.
  For this site we focus on (but not limited too)

* [init](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/src/init.cpp)
* [validation](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/src/validation.cpp)
* [consensus](https://github.com/bitcoin/bitcoin/tree/master/src/consensus)
* [net](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/src/net.cpp)
* [addrman](https://github.com/pro-bitcoin/pro-bitcoin/blob/prometheus/src/addrman.cpp)

---

## Additional Resources

* [Lexicon](https://btcinformation.org/en/vocabulary) & [Vocabulary](https://river.com/learn/terms/)
* [Khan Academy](https://www.khanacademy.org/economics-finance-domain/core-finance/money-and-banking/bitcoin/v/bitcoin-what-is-it)
* [glozow's personal notes](https://github.com/glozow/bitcoin-notes)
* [Review Club](https://bitcoincore.reviews/)
* [CONTRIBUTING.md](https://github.com/bitcoin/bitcoin/blob/master/CONTRIBUTING.md)
* [CPP](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
* [Bitcoin Optech](https://bitcoinops.org/)

### Videos Lectures

* [The Road to Full-Time Bitcoin | Gloria Zhao](https://www.youtube.com/watch?v=lffDLQGMkVI)
* [MIT course](https://www.youtube.com/watch?v=IJquEYhiq_U&list=PLUl4u3cNGP61KHzhg3JIJdK08JLSlcLId)
* [Coursera](https://www.coursera.org/learn/cryptocurrency)
* [Chaincode Seminars](https://learning.chaincode.com/)

### Podcasts

* [Chaincode](https://podcast.chaincode.com/)
