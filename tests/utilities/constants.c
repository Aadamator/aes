#include "constants.h"
#include "macros.h"
#include "types.h"

const char ciphertext_as_hex[33] = "3925841d02dc09fbdc118597196a0b32";
const char key_as_hex[33] = "2b7e151628aed2a6abf7158809cf4f3c";
const char plaintext_as_hex[33] = "3243f6a8885a308d313198a2e0370734";

const RoundState round_states[NUM_ROUNDS] = {
    // R[01]
    {
        "193de3bea0f4e22b9ac68d2ae9f84808", // start
        "d42711aee0bf98f1b8b45de51e415230", // s_box
        "d4bf5d30e0b452aeb84111f11e2798e5", // s_row
        "046681e5e0cb199a48f8d37a2806264c", // m_col
        "a0fafe1788542cb123a339392a6c7605", // k_sch
    },
    // R[02]
    {
        "a49c7ff2689f352b6b5bea43026a5049",
        "49ded28945db96f17f39871a7702533b",
        "49db873b453953897f02d2f177de961a",
        "584dcaf11b4b5aacdbe7caa81b6bb0e5",
        "f2c295f27a96b9435935807a7359f67f",
    },
    // R[03]
    {
        "aa8f5f0361dde3ef82d24ad26832469a",
        "ac73cf7befc111df13b5d6b545235ab8",
        "acc1d6b8efb55a7b1323cfdf457311b5",
        "75ec0993200b633353c0cf7cbb25d0dc",
        "3d80477d4716fe3e1e237e446d7a883b",
    },
    // R[04]
    {
        "486c4eee671d9d0d4de3b138d65f58e7",
        "52502f2885a45ed7e311c807f6cf6a94",
        "52a4c89485116a28e3cf2fd7f6505e07",
        "0fd6daa9603138bf6fc0106b5eb31301",
        "ef44a541a8525b7fb671253bdb0bad00",
    },
    // R[05]
    {
        "e0927fe8c86363c0d9b1355085b8be01",
        "e14fd29be8fbfbba35c89653976cae7c",
        "e1fb967ce8c8ae9b356cd2ba974ffb53",
        "25d1a9adbd11d168b63a338e4c4cc0b0",
        "d4d1c6f87c839d87caf2b8bc11f915bc",
    },
    // R[06]
    {
        "f1006f55c1924cef7cc88b325db5d50c",
        "a163a8fc784f29df10e83d234cd503fe",
        "a14f3dfe78e803fc10d5a8df4c632923",
        "4b868d6d2c4a8980339df4e837d218d8",
        "6d88a37a110b3efddbf98641ca0093fd",
    },
    // R[07]
    {
        "260e2e173d41b77de86472a9fdd28b25",
        "f7ab31f02783a9ff9b4340d354b53d3f",
        "f783403f27433df09bb531ff54aba9d3",
        "1415b5bf461615ec274656d7342ad843",
        "4e54f70e5f5fc9f384a64fb24ea6dc4f",
    },
    // R[08]
    {
        "5a4142b11949dc1fa3e019657a8c040c",
        "be832cc8d43b86c00ae1d44dda64f2fe",
        "be3bd4fed4e1f2c80a642cc0da83864d",
        "00512fd1b1c889ff54766dcdfa1b99ea",
        "ead27321b58dbad2312bf5607f8d292f",
    },
    // R[09]
    {
        "ea835cf00445332d655d98ad8596b0c5",
        "87ec4a8cf26ec3d84d4c46959790e7a6",
        "876e46a6f24ce78c4d904ad897ecc395",
        "473794ed40d4e4a5a3703aa64c9f42bc",
        "ac7766f319fadc2128d12941575c006e",
    },
    // R[10]
    {
        "eb40f21e592e38848ba113e71bc342d2",
        "e9098972cb31075f3d327d94af2e2cb5",
        "e9317db5cb322c723d2e895faf090794",
        "", // no mix column for round 10
        "d014f9a8c9ee2589e13f0cc8b6630ca6",
    },
};
