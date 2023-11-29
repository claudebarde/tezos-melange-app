type t;
type bigmap_abstraction;

type block_header_response = {
    chain_id: string,
    hash: string,
    level: int,
    timestamp: string
};

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

    [@mel.get] external rpc: t => RpcClient.t;
    [@mel.send] external set_wallet_provider: (t, BeaconWallet.t) => unit = "setWalletProvider";
    [@mel.get] external tz_provider: t => TzProvider.t = "tz"
    [@mel.get] external contract: t => ContractProvider.t = "contract"
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


