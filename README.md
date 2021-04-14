# cryptopals-cpp

Matasano crypto challenges (https://cryptopals.com/) in cpp, with Boost for some stuff.

## Requirements ##

g++, boost, pthreads (for some challenges), ninja

## Usage ##

``` bash
git clone https://github.com/plaxi0s/cryptopals-cpp.git
cd cryptopals-cpp
meson build --wipe
cd build
# use meson configure to specify which set/file to compile.
meson configure -Dset=set1 -Dfile=3
ninja
./3

```
## Set 1

  - [X] [Convert hex to base64](https://cryptopals.com/sets/1/challenges/1)
  - [X] [Fixed XOR](https://cryptopals.com/sets/1/challenges/2)
  - [X] [Single-byte XOR cipher](https://cryptopals.com/sets/1/challenges/3)
  - [X] [Detect single-character XOR](https://cryptopals.com/sets/1/challenges/4)
  - [X] [Implement repeating-key XOR](https://cryptopals.com/sets/1/challenges/5)
  - [X] [Break repeating-key XOR](https://cryptopals.com/sets/1/challenges/6)
  - [X] [AES in ECB mode](https://cryptopals.com/sets/1/challenges/7)
  - [X] [Detect AES in ECB mode](https://cryptopals.com/sets/1/challenges/8)

## Set 2

  - [X] [Implement PKCS#7 padding](https://cryptopals.com/sets/2/challenges/9)
  - [X] [Implement CBC mode](https://cryptopals.com/sets/2/challenges/10)
  - [X] [An ECB/CBC detection oracle](https://cryptopals.com/sets/2/challenges/11)
  - [X] [Byte-at-a-time ECB decryption (Simple)](https://cryptopals.com/sets/2/challenges/12)
  - [X] [ECB cut-and-paste](https://cryptopals.com/sets/2/challenges/13)
  - [X] [Byte-at-a-time ECB decryption (Harder)](https://cryptopals.com/sets/2/challenges/14)
  - [X] [PKCS#7 padding validation](https://cryptopals.com/sets/2/challenges/15)
  - [X] [CBC bitflipping attacks](https://cryptopals.com/sets/2/challenges/16)

## Set 3

  - [X] [The CBC padding oracle](https://cryptopals.com/sets/3/challenges/17)
  - [X] [Implement CTR, the stream cipher mode](https://cryptopals.com/sets/3/challenges/18)
  - [X] [Break fixed-nonce CTR mode using substitions](https://cryptopals.com/sets/3/challenges/19)
  - [X] [Break fixed-nonce CTR statistically](https://cryptopals.com/sets/3/challenges/20)
  - [X] [Implement the MT19937 Mersenne Twister RNG](https://cryptopals.com/sets/3/challenges/21)
  - [X] [Crack an MT19937 seed](https://cryptopals.com/sets/3/challenges/22)
  - [X] [Clone an MT19937 RNG from its output](https://cryptopals.com/sets/3/challenges/23)
  - [X] [Create the MT19937 stream cipher and break it](https://cryptopals.com/sets/3/challenges/24)

## Set 4

  - [X] [Break "random access read/write" AES CTR](https://cryptopals.com/sets/4/challenges/25)
  - [X] [CTR bitflipping](https://cryptopals.com/sets/4/challenges/26)
  - [X] [Recover the key from CBC with IV=Key](https://cryptopals.com/sets/4/challenges/27)
  - [X] [Implement a SHA-1 keyed MAC](https://cryptopals.com/sets/4/challenges/28)
  - [X] [Break a SHA-1 keyed MAC using length extension](https://cryptopals.com/sets/4/challenges/29)
  - [X] [Break an MD4 keyed MAC using length extension](https://cryptopals.com/sets/4/challenges/30)
  - [X] [Implement and break HMAC-SHA1 with an artificial timing leak](https://cryptopals.com/sets/4/challenges/31)
  - [X] [Break HMAC-SHA1 with a slightly less artificial timing leak](https://cryptopals.com/sets/4/challenges/32)

## Set 5

  - [X] [Implement Diffie-Hellman](https://cryptopals.com/sets/5/challenges/33)
  - [ ] [Implement a MITM key-fixing attack on Diffie-Hellman with parameter injection](https://cryptopals.com/sets/5/challenges/34)
  - [ ] [Implement DH with negotiated groups, and break with malicious "g" parameters](https://cryptopals.com/sets/5/challenges/35)
  - [ ] [Implement Secure Remote Password (SRP)](https://cryptopals.com/sets/5/challenges/36)
  - [ ] [Break SRP with a zero key](https://cryptopals.com/sets/5/challenges/37)
  - [ ] [Offline dictionary attack on simplified SRP](https://cryptopals.com/sets/5/challenges/38)
  - [ ] [Implement RSA](https://cryptopals.com/sets/5/challenges/39)
  - [ ] [Implement an E=3 RSA Broadcast attack](https://cryptopals.com/sets/5/challenges/40)

## Set 6

  - [ ] [Implement unpadded message recovery oracle](https://cryptopals.com/sets/6/challenges/41)
  - [ ] [Bleichenbacher's e=3 RSA Attack](https://cryptopals.com/sets/6/challenges/42)
  - [ ] [DSA key recovery from nonce](https://cryptopals.com/sets/6/challenges/43)
  - [ ] [DSA nonce recovery from repeated nonce](https://cryptopals.com/sets/6/challenges/44)
  - [ ] [DSA parameter tampering](https://cryptopals.com/sets/6/challenges/45)
  - [ ] [RSA parity oracle](https://cryptopals.com/sets/6/challenges/46)
  - [ ] [Bleichenbacher's PKCS 1.5 Padding Oracle (Simple Case)](https://cryptopals.com/sets/6/challenges/47)
  - [ ] [Bleichenbacher's PKCS 1.5 Padding Oracle (Complete Case)](https://cryptopals.com/sets/6/challenges/48)

## Set 7
  
  - [ ] [CBC-MAC Message Forgery](https://cryptopals.com/sets/7/challenges/49)
  - [ ] [Hashing with CBC-MAC](https://cryptopals.com/sets/7/challenges/50)
  - [ ] [Compression Ratio Side-Channel Attacks](https://cryptopals.com/sets/7/challenges/51)
  - [ ] [Iterated Hash Function Multicollisions](https://cryptopals.com/sets/7/challenges/52)
  - [ ] [Kelsey and Schneier's Expandable Messages](https://cryptopals.com/sets/7/challenges/53)
  - [ ] [Kelsey and Kohno's Nostradamus Attack](https://cryptopals.com/sets/7/challenges/54)
  - [ ] [MD4 Collisions](https://cryptopals.com/sets/7/challenges/55)
  - [ ] [RC4 Single-Byte Biases](https://cryptopals.com/sets/7/challenges/56)


---
