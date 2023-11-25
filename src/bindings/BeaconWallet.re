// BEACON WALLET
type client;

type t = {
    client: client
};

type wallet_options;
[@mel.obj]
external new_wallet_options: (
    ~name: string,
    ~description: string=?,
    unit
) => wallet_options;

// type permissions_options;
// [@mel.obj]
// external new_permissions_options: (
//     ~network: string=?,
//     ~_type: {..}=?,
//     unit
// ) => permissions_options;

type account_info = {.
    "accountIdentifier": string,
    "senderId": string,
    "publicKey": string,
    "address": string,
    "connectedAt": int,
};

[@mel.new] [@mel.module "@taquito/beacon-wallet"] 
    external new_wallet: wallet_options => t = "BeaconWallet"

[@mel.send] external request_permissions: t => Js.Promise.t(unit) = "requestPermissions";
[@mel.send] external get_pkh: t => Js.Promise.t(string) = "getPKH";

[@mel.send] external get_active_account: client => Js.Promise.t(Js.Nullable.t(account_info)) = "getActiveAccount";
[@mel.send] external clear_active_account: client => Js.Promise.t(unit) = "clearActiveAccount";