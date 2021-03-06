// Copyright (c) 2013 The Bitcoin Core developers
<<<<<<< HEAD
// Distributed under the MIT/X11 software license, see the accompanying
=======
// Distributed under the MIT software license, see the accompanying
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for alert system
//

#include "alert.h"
#include "chain.h"
#include "chainparams.h"
#include "clientversion.h"
#include "data/alertTests.raw.h"

<<<<<<< HEAD
#include "key.h"
=======
#include "main.h"
>>>>>>> f568462ca04b73485d7e41266a2005155ff69707
#include "serialize.h"
#include "streams.h"
#include "util.h"
#include "utilstrencodings.h"

#include "test/test_bitcoin.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

/*this test relies on signed alerts in src/test/data/alertTests.raw

how to create this data:
1. set secretKey below to the secret key belonging to the mainnet vAlertPubKey from chainparams.cpp
2. delete the file src/test/alertTests.raw if it exists
3. compile the test suite with CREATE_ALERTS defined
4. execute this test suite
5. move the now created file src/test/data/alertTests.raw to src/test/data/alertTests.raw
6. undefine CREATE_ALERTS and compile the test suite again
*/

#ifdef CREATE_ALERTS
struct CreateAlerts
{
    void SignAndSave(CAlert alert, const std::string filename) {
        //serialze alert message
        CDataStream sMsg(SER_NETWORK, PROTOCOL_VERSION);
        sMsg << (CUnsignedAlert)alert;
        alert.vchMsg.reserve(sMsg.size());
        for(size_t i=0; i<sMsg.size(); i++) {
            alert.vchMsg.push_back(sMsg[i]);
        }

        //a dummy secret key with the public key
        //0469204F0E1800E16C1F85176BDC27A245F09987DB71A1EF5C4BD48A42F9AFD1D74F21469488DB552B594AC29CE667AD60DAAD0FFBCE03FB0C2AC49FFB07B36DC5
        //set to match the mainnet vAlertPubKey from chainparams.cpp
        const std::vector<unsigned char> secretKey = ParseHex("1A4976EE6174D80B8B3FF5E9B9C6E638CABFA9A5975557FEA967BC089A5584EE");
        CKey secret;
        secret.Set(secretKey.begin(), secretKey.end(), false);
        assert(secret.IsValid());
        
        //sign alert
        secret.Sign(alert.GetHash(), alert.vchSig);
        assert(alert.CheckSignature());
        
        //serialize alert
        CDataStream ss(SER_DISK, CLIENT_VERSION);
        ss << alert;
        
        //write alert
        std::ofstream fs;
        fs.open(filename.c_str(), std::ios::out | std::ios::app | std::ios::binary);
        fs.write((char*)&ss[0], ss.size());
        fs.close();
    }

    //
    // alertTests contains 7 alerts, generated with this code:
    //
    CreateAlerts()
    {
        std::string filename("src/test/alertTests.raw");

        CAlert alert;
        alert.nRelayUntil   = 60;
        alert.nExpiration   = 24 * 60 * 60;
        alert.nID           = 1;
        alert.nCancel       = 0;   // cancels previous messages up to this ID number
        alert.nMinVer       = 0;  // These versions are protocol versions
        alert.nMaxVer       = 999001;
        alert.nPriority     = 1;
        alert.strComment    = "Alert comment";
        alert.strStatusBar  = "Alert 1";

        SignAndSave(alert, filename);

        alert.setSubVer.insert(std::string("/Satoshi:0.1.0/"));
        alert.strStatusBar  = "Alert 1 for Satoshi 0.1.0";
        SignAndSave(alert, filename);

        alert.setSubVer.insert(std::string("/Satoshi:0.2.0/"));
        alert.strStatusBar  = "Alert 1 for Satoshi 0.1.0, 0.2.0";
        SignAndSave(alert, filename);

        alert.setSubVer.clear();
        ++alert.nID;
        alert.nCancel = 1;
        alert.nPriority = 100;
        alert.strStatusBar  = "Alert 2, cancels 1";
        SignAndSave(alert, filename);

        alert.nExpiration += 60;
        ++alert.nID;
        SignAndSave(alert, filename);

        ++alert.nID;
        alert.nMinVer = 11;
        alert.nMaxVer = 22;
        SignAndSave(alert, filename);

        ++alert.nID;
        alert.strStatusBar  = "Alert 2 for Satoshi 0.1.0";
        alert.setSubVer.insert(std::string("/Satoshi:0.1.0/"));
        SignAndSave(alert, filename);

        ++alert.nID;
        alert.nMinVer = 0;
        alert.nMaxVer = 999999;
        alert.strStatusBar  = "Evil Alert'; /bin/ls; echo '";
        alert.setSubVer.clear();
        SignAndSave(alert, filename);
    }
    ~CreateAlerts() { }
};

BOOST_GLOBAL_FIXTURE(CreateAlerts)
#endif

struct ReadAlerts : public TestingSetup
{
    ReadAlerts()
    {
        std::vector<unsigned char> vch(alert_tests::alertTests, alert_tests::alertTests + sizeof(alert_tests::alertTests));
        CDataStream stream(vch, SER_DISK, CLIENT_VERSION);
        try {
            while (!stream.eof())
            {
                CAlert alert;
                stream >> alert;
                alerts.push_back(alert);
            }
        }
        catch (const std::exception&) { }
    }
    ~ReadAlerts() { }

    static std::vector<std::string> read_lines(boost::filesystem::path filepath)
    {
        std::vector<std::string> result;

        std::ifstream f(filepath.string().c_str());
        std::string line;
        while (std::getline(f,line))
            result.push_back(line);

        return result;
    }

    std::vector<CAlert> alerts;
};

BOOST_FIXTURE_TEST_SUITE(Alert_tests, ReadAlerts)


BOOST_AUTO_TEST_CASE(AlertApplies)
{
    SetMockTime(11);
    const std::vector<unsigned char>& alertKey = Params(CBaseChainParams::MAIN).AlertKey();

    BOOST_FOREACH(const CAlert& alert, alerts)
    {
        BOOST_CHECK(alert.CheckSignature(alertKey));
    }

    BOOST_CHECK(alerts.size() >= 3);

    // Matches:
    BOOST_CHECK(alerts[0].AppliesTo(1, ""));
    BOOST_CHECK(alerts[0].AppliesTo(999001, ""));
    BOOST_CHECK(alerts[0].AppliesTo(1, "/Satoshi:11.11.11/"));

    BOOST_CHECK(alerts[1].AppliesTo(1, "/Satoshi:0.1.0/"));
    BOOST_CHECK(alerts[1].AppliesTo(999001, "/Satoshi:0.1.0/"));

    BOOST_CHECK(alerts[2].AppliesTo(1, "/Satoshi:0.1.0/"));
    BOOST_CHECK(alerts[2].AppliesTo(1, "/Satoshi:0.2.0/"));

    // Don't match:
    BOOST_CHECK(!alerts[0].AppliesTo(-1, ""));
    BOOST_CHECK(!alerts[0].AppliesTo(999002, ""));

    BOOST_CHECK(!alerts[1].AppliesTo(1, ""));
    BOOST_CHECK(!alerts[1].AppliesTo(1, "Satoshi:0.1.0"));
    BOOST_CHECK(!alerts[1].AppliesTo(1, "/Satoshi:0.1.0"));
    BOOST_CHECK(!alerts[1].AppliesTo(1, "Satoshi:0.1.0/"));
    BOOST_CHECK(!alerts[1].AppliesTo(-1, "/Satoshi:0.1.0/"));
    BOOST_CHECK(!alerts[1].AppliesTo(999002, "/Satoshi:0.1.0/"));
    BOOST_CHECK(!alerts[1].AppliesTo(1, "/Satoshi:0.2.0/"));

    BOOST_CHECK(!alerts[2].AppliesTo(1, "/Satoshi:0.3.0/"));

    SetMockTime(0);
}


BOOST_AUTO_TEST_CASE(AlertNotify)
{
    SetMockTime(11);
    const std::vector<unsigned char>& alertKey = Params(CBaseChainParams::MAIN).AlertKey();

    boost::filesystem::path temp = GetTempPath() / "alertnotify.txt";
    boost::filesystem::remove(temp);

    mapArgs["-alertnotify"] = std::string("echo %s >> ") + temp.string();

    BOOST_FOREACH(CAlert alert, alerts)
        alert.ProcessAlert(alertKey, false);

    std::vector<std::string> r = read_lines(temp);
    BOOST_CHECK_EQUAL(r.size(), 4u);

// Windows built-in echo semantics are different than posixy shells. Quotes and
// whitespace are printed literally.

#ifndef WIN32
    BOOST_CHECK_EQUAL(r[0], "Alert 1");
    BOOST_CHECK_EQUAL(r[1], "Alert 2, cancels 1");
    BOOST_CHECK_EQUAL(r[2], "Alert 2, cancels 1");
    BOOST_CHECK_EQUAL(r[3], "Evil Alert; /bin/ls; echo "); // single-quotes should be removed
#else
    BOOST_CHECK_EQUAL(r[0], "'Alert 1' ");
    BOOST_CHECK_EQUAL(r[1], "'Alert 2, cancels 1' ");
    BOOST_CHECK_EQUAL(r[2], "'Alert 2, cancels 1' ");
    BOOST_CHECK_EQUAL(r[3], "'Evil Alert; /bin/ls; echo ' ");
#endif
    boost::filesystem::remove(temp);

    SetMockTime(0);
}

static bool falseFunc() { return false; }

BOOST_AUTO_TEST_CASE(PartitionAlert)
{
    // Test PartitionCheck
    CCriticalSection csDummy;
    CBlockIndex indexDummy[800];
    CChainParams& params = Params(CBaseChainParams::MAIN);
    int64_t nPowTargetSpacing = params.GetConsensus(0).nPowTargetSpacing;

    // Generate fake blockchain timestamps relative to
    // an arbitrary time:
    int64_t now = 1427379054;
    SetMockTime(now);
    for (int i = 0; i < 800; i++)
    {
        indexDummy[i].phashBlock = NULL;
        if (i == 0) indexDummy[i].pprev = NULL;
        else indexDummy[i].pprev = &indexDummy[i-1];
        indexDummy[i].nHeight = i;
        indexDummy[i].nTime = now - (800-i)*nPowTargetSpacing;
        // Other members don't matter, the partition check code doesn't
        // use them
    }

    // Test 1: chain with blocks every nPowTargetSpacing seconds,
    // as normal, no worries:
    PartitionCheck(falseFunc, csDummy, &indexDummy[799], nPowTargetSpacing);
    BOOST_CHECK(strMiscWarning.empty());

    // Test 2: go 3.5 hours without a block, expect a warning:
    now += 3*60*60+30*60;
    SetMockTime(now);
    PartitionCheck(falseFunc, csDummy, &indexDummy[799], nPowTargetSpacing);
    BOOST_CHECK(!strMiscWarning.empty());
    BOOST_TEST_MESSAGE(std::string("Got alert text: ")+strMiscWarning);
    strMiscWarning = "";

    // Test 3: test the "partition alerts only go off once per day"
    // code:
    now += 60*10;
    SetMockTime(now);
    PartitionCheck(falseFunc, csDummy, &indexDummy[799], nPowTargetSpacing);
    BOOST_CHECK(strMiscWarning.empty());

    // Test 4: get 2.5 times as many blocks as expected:
    now += 60*60*24; // Pretend it is a day later
    SetMockTime(now);
    int64_t quickSpacing = nPowTargetSpacing*2/5;
    for (int i = 0; i < 800; i++) // Tweak chain timestamps:
        indexDummy[i].nTime = now - (800-i)*quickSpacing;
    PartitionCheck(falseFunc, csDummy, &indexDummy[799], nPowTargetSpacing);
    BOOST_CHECK(!strMiscWarning.empty());
    BOOST_TEST_MESSAGE(std::string("Got alert text: ")+strMiscWarning);
    strMiscWarning = "";

    SetMockTime(0);
}

BOOST_AUTO_TEST_SUITE_END()
