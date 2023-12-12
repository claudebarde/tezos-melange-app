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

type transfer_param = {
    [@mel.as "to"]
    to_: string,
    amount: float
};

// WALLET OPERATION
module WalletOperation = {
    type t = {
        opHash: string
    };

    [@mel.get] external hash: t => string = "opHash"
    [@mel.send] external confirmation: (t, ~confirmations: int=?, ~timeout: int=?) => Js.Promise.t(float);
}

// WALLET SEND
module WalletSend = {
    type t;

    [@mel.send] external send: t => Js.Promise.t(WalletOperation.t);
};

[@mel.new] [@mel.module "@taquito/beacon-wallet"] 
    external new_wallet: wallet_options => t = "BeaconWallet"

[@mel.send] external request_permissions: t => Js.Promise.t(unit) = "requestPermissions";
[@mel.send] external get_pkh: t => Js.Promise.t(string) = "getPKH";

[@mel.send.pipe: t] external transfer: transfer_param => WalletSend.t = "transfer";

[@mel.send] external get_active_account: client => Js.Promise.t(Js.Nullable.t(account_info)) = "getActiveAccount";
[@mel.send] external clear_active_account: client => Js.Promise.t(unit) = "clearActiveAccount";