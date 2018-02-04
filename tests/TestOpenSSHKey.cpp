/*
 *  Copyright (C) 2017 Toni Spets <toni.spets@iki.fi>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TestOpenSSHKey.h"
#include "TestGlobal.h"
#include "sshagent/OpenSSHKey.h"
#include "crypto/Crypto.h"

QTEST_GUILESS_MAIN(TestOpenSSHKey)

void TestOpenSSHKey::initTestCase()
{
    QVERIFY(Crypto::init());
}

void TestOpenSSHKey::testParse()
{
    // mixed line endings and missing ones are intentional, we only require 3 lines total
    const QString keyString = QString(
        "\r\n\r"
        "-----BEGIN OPENSSH PRIVATE KEY-----\n"
        "b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW"
        "QyNTUxOQAAACDdlO5F2kF2WzedrBAHBi9wBHeISzXZ0IuIqrp0EzeazAAAAKjgCfj94An4"
        "/QAAAAtzc2gtZWQyNTUxOQAAACDdlO5F2kF2WzedrBAHBi9wBHeISzXZ0IuIqrp0EzeazA"
        "AAAEBe1iilZFho8ZGAliiSj5URvFtGrgvmnEKdiLZow5hOR92U7kXaQXZbN52sEAcGL3AE"
        "d4hLNdnQi4iqunQTN5rMAAAAH29wZW5zc2hrZXktdGVzdC1wYXJzZUBrZWVwYXNzeGMBAg"
        "MEBQY=\r"
        "-----END OPENSSH PRIVATE KEY-----\r\n\r"
    );

    const QByteArray keyData = keyString.toLatin1();

    OpenSSHKey key;
    QVERIFY(key.parse(keyData));
    QVERIFY(!key.encrypted());
    QCOMPARE(key.cipherName(), QString("none"));
    QCOMPARE(key.type(), QString("ssh-ed25519"));
    QCOMPARE(key.comment(), QString("opensshkey-test-parse@keepassxc"));
    QCOMPARE(key.fingerprint(), QString("SHA256:D1fVmA15YXzaJ5sdO9dXxo5coHL/pnNaIfCvokHzTA4"));

    QByteArray publicKey, privateKey;
    BinaryStream publicStream(&publicKey), privateStream(&privateKey);

    QVERIFY(key.writePublic(publicStream));
    QVERIFY(key.writePrivate(privateStream));

    QVERIFY(publicKey.length() == 51);
    QVERIFY(privateKey.length() == 154);
}

void TestOpenSSHKey::testParseDSA()
{
    const QString keyString = QString(
        "-----BEGIN DSA PRIVATE KEY-----\n"
        "MIIBuwIBAAKBgQCudjbvSh8JxQOr2laCqZM1t4kNWBETVOXz5vgk9iw6Z5opB9/k\n"
        "g4nFc1PVq7fdAIc8W/5WCAjugKcxPb9PIHfcwY2fimmiPWFK68/eHKLoCuIn2wxB\n"
        "63ig2hAhx5U5aYG9QHkNCaT6VX7rc19nToSeZXlpja4x54/DaQaqOEWYsQIVAOer\n"
        "UQWfccz7KXUu6+x7heGob6I3AoGAVDRFJIlL0DI/4nePIcgwgwbfgs2ojSu21g4w\n"
        "dQoXvqU34XydPgPQ985XIIuiDkaomRw4yYd/Sh4ZapFcrP++iJ1V+WS6kLcWPHMq\n"
        "poYwk8mq6GLbPFLEjr+n6HgX5ln15n3i4WAopNH7mEl0glY9L0rxmcN0XOpqw6Ux\n"
        "ETGEfAwCgYAiOeYwblMkkTIGtVx5NvNsOlfrBYL4GqUP9oQMO5I+xLZLWQIf+7Jp\n"
        "8t6mwxSBz0RHjNVQ11vZowNjq3587aLy57bVwf2lIm9KSvS6z9HoNbHgQimcBorR\n"
        "J9l9RUrj7TnsZgiVw66j2r34nHRHRtggiO+qrMtw7MJc0Q7jiuTmzgIVAMXbk0T9\n"
        "nBfSLWQz/L8RexU2GR4e\n"
        "-----END DSA PRIVATE KEY-----\n"
    );

    const QByteArray keyData = keyString.toLatin1();

    OpenSSHKey key;
    QVERIFY(key.parse(keyData));
    QVERIFY(!key.encrypted());
    QCOMPARE(key.cipherName(), QString("none"));
    QCOMPARE(key.type(), QString("ssh-dss"));
    QCOMPARE(key.comment(), QString(""));
    QCOMPARE(key.fingerprint(), QString("SHA256:tbbNuLN1hja8JNASDTlLOZQsbTlJDzJlz/oAGK3sX18"));
}

void TestOpenSSHKey::testParseRSA()
{
    const QString keyString = QString(
        "-----BEGIN RSA PRIVATE KEY-----\n"
        "MIIEpAIBAAKCAQEAsCHtJicDPWnvHSIKbnTZaJkIB9vgE0pmLdK580JUqBuonVbB\n"
        "y1QTy0ZQ7/TtqvLPgwPK88TR46OLO/QGCzo2+XxgJ85uy0xfuyUYRmSuw0drsErN\n"
        "mH8vU91lSBxsGDp9LtBbgHKoR23vMWZ34IxFRc55XphrIH48ijsMaL6bXBwF/3tD\n"
        "9T3lm2MpP1huyVNnIY9+GRRWCy4f9LMj/UGu/n4RtwwfpOZBBRwYkq5QkzA9lPm/\n"
        "VzF3MP1rKTMkvAw+Nfb383mkmc6MRnsa6uh6iDa9aVB7naegM13UJQX/PY1Ks6pO\n"
        "XDpy/MQ7iCh+HmYNq5dRmARyaNl9xIXJNhz1cQIDAQABAoIBAQCnEUc1LUQxeM5K\n"
        "wANNCqE+SgoIClPdeHC7fmrLh1ttqe6ib6ybBUFRS31yXs0hnfefunVEDKlaV8K2\n"
        "N52UAMAsngFHQNRvGh6kEWeZPd9Xc+N98TZbNCjcT+DGKc+Om8wqH5DrodZlCq4c\n"
        "GaoT4HnE4TjWtZTH2XXrWF9I66PKFWf070R44nvyVcvaZi4pC2YmURRPuGF6K1iK\n"
        "dH8zM6HHG1UGu2W6hLNn+K01IulG0Lb8eWNaNYMmtQWaxyp7I2IWkkecUs3nCuiR\n"
        "byFOoomCjdh8r9yZFvwxjGUhgtkALN9GCU0Mwve+s11IB2gevruN+q9/Qejbyfdm\n"
        "IlgLAeTRAoGBANRcVzW9CYeobCf+U9hKJFEOur8XO+J2mTMaELA0EjWpTJFAeIT7\n"
        "KeRpCRG4/vOSklxxRF6vP1EACA4Z+5BlN+FTipHHs+bSEgqkPZiiANDH7Zot5Iqv\n"
        "1q0fRyldNRZNZK7DWp08BPNVWGA/EnEuKJiURxnxBaxNXbUyMCdjxvMvAoGBANRT\n"
        "utbrqS/bAa/DcHKn3V6DRqBl3TDOfvCNjiKC84a67F2uXgzLIdMktr4d1NyCZVJd\n"
        "7/zVgWORLIdg1eAi6rYGoOvNV39wwga7CF+m9sBY0wAaKYCELe6L26r4aQHVCX6n\n"
        "rnIgUv+4o4itmU2iP0r3wlmDC9pDRQP82vfvQPlfAoGASwhleANW/quvq2HdViq8\n"
        "Mje2HBalfhrRfpDTHK8JUBSFjTzuWG42GxJRtgVbb8x2ElujAKGDCaetMO5VSGu7\n"
        "Fs5hw6iAFCpdXY0yhl+XUi2R8kwM2EPQ4lKO3jqkq0ClNmqn9a5jQWcCVt9yMLNS\n"
        "fLbHeI8EpiCf34ngIcrLXNkCgYEAzlcEZuKkC46xB+dNew8pMTUwSKZVm53BfPKD\n"
        "44QRN6imFbBjU9mAaJnwQbfp6dWKs834cGPolyM4++MeVfB42iZ88ksesgmZdUMD\n"
        "szkl6O0pOJs0I+HQZVdjRbadDZvD22MHQ3+oST1dJ3FVXz3Cdo9qPuT8esMO6f4r\n"
        "qfDH2s8CgYAXC/lWWHQ//PGP0pH4oiEXisx1K0X1u0xMGgrChxBRGRiKZUwNMIvJ\n"
        "TqUu7IKizK19cLHF/NBvxHYHFw+m7puNjn6T1RtRCUjRZT7Dx1VHfVosL9ih5DA8\n"
        "tpbZA5KGKcvHtB5DDgT0MHwzBZnb4Q//Rhovzn+HXZPsJTTgHHy3NQ==\n"
        "-----END RSA PRIVATE KEY-----\n"
    );

    const QByteArray keyData = keyString.toLatin1();

    OpenSSHKey key;
    QVERIFY(key.parse(keyData));
    QVERIFY(!key.encrypted());
    QCOMPARE(key.cipherName(), QString("none"));
    QCOMPARE(key.type(), QString("ssh-rsa"));
    QCOMPARE(key.comment(), QString(""));
    QCOMPARE(key.fingerprint(), QString("SHA256:DYdaZciYNxCejr+/8x+OKYxeTU1D5UsuIFUG4PWRFkk"));
}

void TestOpenSSHKey::testDecryptAES256CBC()
{
    const QString keyString = QString(
        "-----BEGIN OPENSSH PRIVATE KEY-----\n"
        "b3BlbnNzaC1rZXktdjEAAAAACmFlczI1Ni1jYmMAAAAGYmNyeXB0AAAAGAAAABD2A0agtd\n"
        "oGtJiI9JvIxYbTAAAAEAAAAAEAAAAzAAAAC3NzaC1lZDI1NTE5AAAAIDPvDXmi0w1rdMoX\n"
        "fOeyZ0Q/v+wqq/tPFgJwxnW5ADtfAAAAsC3UPsf035hrF5SgZ48p55iDFPiyGfZC/C3vQx\n"
        "+THzpQo8DTUmFokdPn8wvDYGQoIcr9q0RzJuKV87eMQf3zzvZfJthtLYBlt330Deivv9AQ\n"
        "MbKdhPZ4SfwRvv0grgT2EVId3GQAPgSVBhXYQTOf2CdmbXV4kieFLTmSsBMy+v6Qn5Rqur\n"
        "PDWBwuLQgamcVDZuhrkUEqIVJZU2zAiRU2oAXsw/XOgFV6+Y5UZmLwWJQZ\n"
        "-----END OPENSSH PRIVATE KEY-----\n"
    );

    const QByteArray keyData = keyString.toLatin1();

    OpenSSHKey key;
    QVERIFY(key.parse(keyData));
    QVERIFY(key.encrypted());
    QCOMPARE(key.cipherName(), QString("aes256-cbc"));
    QVERIFY(!key.openPrivateKey("incorrectpassphrase"));
    QVERIFY(key.openPrivateKey("correctpassphrase"));
    QCOMPARE(key.type(), QString("ssh-ed25519"));
    QCOMPARE(key.comment(), QString("opensshkey-test-aes256cbc@keepassxc"));

    QByteArray publicKey, privateKey;
    BinaryStream publicStream(&publicKey), privateStream(&privateKey);

    QVERIFY(key.writePublic(publicStream));
    QVERIFY(key.writePrivate(privateStream));

    QVERIFY(publicKey.length() == 51);
    QVERIFY(privateKey.length() == 158);
}

void TestOpenSSHKey::testDecryptAES256CTR()
{
    const QString keyString = QString(
        "-----BEGIN OPENSSH PRIVATE KEY-----\n"
        "b3BlbnNzaC1rZXktdjEAAAAACmFlczI1Ni1jdHIAAAAGYmNyeXB0AAAAGAAAABAMhIAypt\n"
        "WP4tZJBmMwq0tTAAAAEAAAAAEAAAAzAAAAC3NzaC1lZDI1NTE5AAAAIErNsS8ROy43XoWC\n"
        "nO9Sn2lEFBJYcDVtRPM1t6WB7W7OAAAAsFKXMOlPILoTmMj2JmcqzjaYAhaCezx18HDp76\n"
        "VrNxaZTd0T28EGFSkzrReeewpJWy/bWlhLoXR5fRyOSSto+iMg/pibIvIJMrD5sqxlxr/e\n"
        "c5lSeSZUzIK8Rv+ou/3EFDcY5jp8hVXqA4qNtoM/3fV52vmwlNje5d1V5Gsr4U8443+i+p\n"
        "swqksozfatkynk51uR/9QFoOJKlsL/Z3LkK1S/apYz/K331iU1f5ozFELf\n"
        "-----END OPENSSH PRIVATE KEY-----\n"
    );

    const QByteArray keyData = keyString.toLatin1();

    OpenSSHKey key;
    QVERIFY(key.parse(keyData));
    QVERIFY(key.encrypted());
    QCOMPARE(key.cipherName(), QString("aes256-ctr"));
    QVERIFY(!key.openPrivateKey("incorrectpassphrase"));
    QVERIFY(key.openPrivateKey("correctpassphrase"));
    QCOMPARE(key.type(), QString("ssh-ed25519"));
    QCOMPARE(key.comment(), QString("opensshkey-test-aes256ctr@keepassxc"));

    QByteArray publicKey, privateKey;
    BinaryStream publicStream(&publicKey), privateStream(&privateKey);

    QVERIFY(key.writePublic(publicStream));
    QVERIFY(key.writePrivate(privateStream));

    QVERIFY(publicKey.length() == 51);
    QVERIFY(privateKey.length() == 158);
}