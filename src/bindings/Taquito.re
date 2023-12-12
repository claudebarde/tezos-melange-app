type t;

type block_header_response = {
    chain_id: string,
    hash: string,
    level: int,
    timestamp: string
};

// BIG MAP ABSTRACTION
module BigMapAbstraction {
    type t;
    type key;
    type value;

    [@mel.get] external id: t => BigNumber.t = "id";

    [@mel.send.pipe: t] external get: key => Js.Promise.t(Js.Nullable.t(value)) = "get";
}

// UUSD CONTRACT
module Uusd {
    module Ledger {
        type t;
        // the type for the key is different on Ghostnet because of a lack of annotations
        // type key = {
        //     owner: string,
        //     token_id: int
        // };
        type key = (string, int);
        type value = BigNumber.t;

        [@mel.send.pipe: t] external get: key => Js.Promise.t(Js.Nullable.t(value)) = "get";
    }

    type storage = {
        total_supply: BigNumber.t,
        ledger: Ledger.t
    };

    let token_id = 0;

    [@mel.get] external ledger: storage => Ledger.t = "ledger";
}

//OPERATION
module Operation = {
    type t = {
        hash: string
    };

    [@mel.send] external confirmation: (t, ~confirmations: int=?, ~timeout: int=?) => Js.Promise.t(float);
}

// RPC CLIENT INTERFACE
module RpcClient {
    type t = {
        chain: string,
        rpc: string
    };

    [@mel.send] external get_block_header: t => Js.Promise.t(block_header_response) = "getBlockHeader"
}

// TZ PROVIDER
module TzProvider {
    type t;

    [@mel.send.pipe: t] external get_balance: string => Js.Promise.t(Js.Nullable.t(BigNumber.t)) = "getBalance";
}

// CONTRACT ABSTRACTION
module Contract {
    type t;

    [@mel.send] external storage: t => Js.Promise.t(Js.Nullable.t({..})) = "storage";
    [@mel.send] external uusd_storage: t => Js.Promise.t(Js.Nullable.t(Uusd.storage)) = "storage";
}

// CONTRACT PROVIDER
module ContractProvider {
    type t = {
        address: string
    };

    type transfer_param = {
        [@mel.as "to"]
        to_: string,
        amount: float
    };

    [@mel.send.pipe: t] external at: string => Js.Promise.t(Js.Nullable.t(Contract.t)) = "at";
    [@mel.send.pipe: t] external transfer: transfer_param => Js.Promise.t(Operation.t) = "transfer";
}

// TEZOS TOOLKIT
module TezosToolkit {
    type t;

    [@mel.get] external rpc: t => RpcClient.t = "rpc";
    [@mel.get] external tz_provider: t => TzProvider.t = "tz"
    [@mel.get] external contract: t => ContractProvider.t = "contract"
    [@mel.get] external wallet: t => BeaconWallet.t = "wallet"
    [@mel.send.pipe: t] external set_wallet_provider: BeaconWallet.t => unit = "setWalletProvider";
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


