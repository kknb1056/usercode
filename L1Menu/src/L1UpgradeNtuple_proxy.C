/** @file Includes the L1UpgradeNtuple.C so that it gets built into the library.
 *
 * The code requires L1UpgradeNtuple, but this is in a macro file and not built into the
 * UserCode/L1TriggerUpgrade package. As such I can't use the normal "use" declarations
 * in the BuildFile to link to it. The only way around it I've found is to build it directly
 * into this library.
 */

using namespace std;

#include <UserCode/L1TriggerUpgrade/macros/L1UpgradeNtuple.C>
