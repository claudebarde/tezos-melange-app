type t;

// RPC CLIENT INTERFACE
module RpcClientInterface {
    type t = {
        get_block_header: {.
            "level": int
        }
    };

    [@mel.get] external get_block_header: t =>
}

// TEZOS TOOLKIT
module TezosToolkit {
    type t;

    [@mel.get] external rpc: t => RpcClientInterface.t;
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


