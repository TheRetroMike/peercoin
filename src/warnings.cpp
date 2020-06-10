// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <warnings.h>

#include <sync.h>
#include <util/string.h>
#include <util/system.h>
#include <util/translation.h>

#include <vector>

static Mutex g_warnings_mutex;
static std::string strMiscWarning GUARDED_BY(g_warnings_mutex);
static bool fLargeWorkForkFound GUARDED_BY(g_warnings_mutex) = false;
static bool fLargeWorkInvalidChainFound GUARDED_BY(g_warnings_mutex) = false;
std::string strMintWarning;

void SetMiscWarning(const std::string& strWarning)
{
    LOCK(g_warnings_mutex);
    strMiscWarning = strWarning;
}

void SetfLargeWorkForkFound(bool flag)
{
    LOCK(g_warnings_mutex);
    fLargeWorkForkFound = flag;
}

bool GetfLargeWorkForkFound()
{
    LOCK(g_warnings_mutex);
    return fLargeWorkForkFound;
}

void SetfLargeWorkInvalidChainFound(bool flag)
{
    LOCK(g_warnings_mutex);
    fLargeWorkInvalidChainFound = flag;
}

std::string GetWarnings(bool verbose)
{
    int nPriority = 0;
    bilingual_str warnings_concise;
    std::vector<bilingual_str> warnings_verbose;

    LOCK(g_warnings_mutex);

    // Pre-release build warning
    if (!CLIENT_VERSION_IS_RELEASE) {
        warnings_concise = _("This is a pre-release test build - use at your own risk - do not use for mining or merchant applications");
        warnings_verbose.emplace_back(warnings_concise);
    }

    // peercoin: wallet lock warning for minting
    if (strMintWarning != "")
    {
        nPriority = 0;
        warnings_concise = strMintWarning;
        warnings_verbose += (warnings_verbose.empty() ? "" : warning_separator) + _(strMintWarning.c_str()).translated;
    }

    // Misc warnings like out of disk space and clock is wrong
    if (!strMiscWarning.empty()) {
        nPriority = 1000;
        warnings_concise = Untranslated(strMiscWarning);
        warnings_verbose.emplace_back(warnings_concise);
    }

    if (fLargeWorkForkFound) {
        nPriority = 2000;
        warnings_concise = _("Warning: The network does not appear to fully agree! Some miners appear to be experiencing issues.");
        warnings_verbose.emplace_back(warnings_concise);
    } else if (fLargeWorkInvalidChainFound) {
        nPriority = 2000;
        warnings_concise = _("Warning: We do not appear to fully agree with our peers! You may need to upgrade, or other nodes may need to upgrade.");
        warnings_verbose.emplace_back(warnings_concise);
    }

    if (verbose) {
        return Join(warnings_verbose, Untranslated("<hr />")).translated;
    }

    return warnings_concise.original;
}
