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

// BEACON WALLET
module BeaconWallet = {
    type t;

    // type new_wallet_options = {
    //     name: string
    // };
    type wallet_options;

    [@mel.obj]
    external new_wallet_options: (
        ~name: string,
        ~description: string=?,
        unit
    ) => wallet_options;

    [@mel.new] [@mel.module "@taquito/beacon-wallet"] 
        external new_wallet: wallet_options => t = "BeaconWallet"
}

// TEZOS TOOLKIT
module TezosToolkit {
    type t;

    [@mel.get] external rpc: t => RpcClient.t;
    [@mel.send] external set_wallet_provider: (t, BeaconWallet.t) => unit = "setWalletProvider";
}

[@mel.new] [@mel.module "@taquito/taquito"] 
    external tezos_toolkit: string => TezosToolkit.t = "TezosToolkit" ;


