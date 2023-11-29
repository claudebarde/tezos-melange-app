type t;

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

module TzProvider {
    type t;

    [@mel.send] external get_balance: (t, string) => Js.Promise.t(Js.Nullable.t(BigNumber.t)) = "getBalance";
}

// TEZOS TOOLKIT
module TezosToolkit {
    type t;

    [@mel.get] external rpc: t => RpcClient.t;
    [@mel.send] external set_wallet_provider: (t, BeaconWallet.t) => unit = "setWalletProvider";
    [@mel.get] external tz_provider: t => TzProvider.t = "tz"
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


