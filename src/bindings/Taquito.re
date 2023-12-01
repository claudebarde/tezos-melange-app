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

    [@mel.send] external get:(t, key) => Js.Promise.t(Js.Nullable.t(value)) = "get";
}

// UUSD CONTRACT
module Uusd {
    module Ledger {
        type t;
        type key = {
            owner: string,
            token_id: int
        };
        type value = BigNumber.t;

        [@mel.send] external get:(t, key) => Js.Promise.t(Js.Nullable.t(value)) = "get";
    }

    type storage = {
        total_supply: BigNumber.t,
        ledger: Ledger.t
    };

    let token_id = 0;

    [@mel.get] external ledger: storage => Ledger.t = "ledger";
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

    [@mel.send] external get_balance: (t, string) => Js.Promise.t(Js.Nullable.t(BigNumber.t)) = "getBalance";
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

    [@mel.send] external at: (t, string) => Js.Promise.t(Js.Nullable.t(Contract.t)) = "at";
}

// TEZOS TOOLKIT
module TezosToolkit {
    type t;

    [@mel.get] external rpc: t => RpcClient.t = "rpc";
    [@mel.send] external set_wallet_provider: (t, BeaconWallet.t) => unit = "setWalletProvider";
    [@mel.get] external tz_provider: t => TzProvider.t = "tz"
    [@mel.get] external contract: t => ContractProvider.t = "contract"
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


