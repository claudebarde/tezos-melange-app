[@react.component]
let make = () => {
    open Utils
    
    let (tezos, set_tezos) = React.useState(() => None)
    let (level, set_level) = React.useState(() => None);
    let (wallet, set_wallet) = React.useState(() => None);
    let (user_address, set_user_address) = React.useState(() => None);
    let (user_xtz_balance, set_user_xtz_balance) = React.useState(() => None);
    let (user_uusd_balance, set_user_uusd_balance) = React.useState(() => None);
    let (selected_token, set_selected_token) = React.useState(() => XTZ);
    let (amount_to_send, set_amount_to_send) = React.useState(() => None);

    React.useEffect0(() => {
        open Taquito;

        let tezos = tezos_toolkit("https://mainnet.ecadinfra.com");
        let _ = set_tezos(_ => Some(tezos));
        let rpc = tezos |> TezosToolkit.rpc;
        let _ = rpc |> RpcClient.get_block_header |> Js.Promise.then_(res => {
        let _ = set_level(_ => Some(res.level));
        Js.Promise.resolve();
        })
        None
    });

    // fetches the user's XTZ and uUSD balances if connected
    React.useEffect1(() => {
        open Taquito;

        switch user_address {
        | None => None
        | Some(address) => {
            switch tezos {
            | None => None
            | Some(tezos) => {
                // fetches the XTZ balance
                let _ = 
                tezos
                -> TezosToolkit.tz_provider
                -> TzProvider.get_balance(address)
                |> Js.Promise.then_(res => {
                    let _ = switch (Js.Nullable.toOption(res)){
                        | None => set_user_xtz_balance(_ => Some(0))
                        | Some(balance) => set_user_xtz_balance(_ => Some(BigNumber.to_number(balance)))
                    };
                    Js.Promise.resolve()
                    });
                // fetches the uUSD balance
                let _ =
                tezos
                -> TezosToolkit.contract
                -> ContractProvider.at(uuusd_address)
                |> Js.Promise.then_(res => {
                    let _ = switch (Js.Nullable.toOption(res)) {
                    | None => Js.log("failed to fetch the contract")
                    | Some(contract) => {
                        let _ = 
                        contract
                        |> Contract.uusd_storage
                        |> Js.Promise.then_(storage => {
                            let _ = switch (Js.Nullable.toOption(storage)) {
                            | None => Js.log("failed to fetch the contract storage")
                            | Some(storage) => {
                                // finds the user's balance in the storage
                                let key: Uusd.Ledger.key = { owner: address, token_id: 0};
                                let _ = 
                                storage
                                ->Uusd.ledger
                                ->Uusd.Ledger.get(key)
                                |> Js.Promise.then_(res => {
                                    let _ = switch(Js.Nullable.toOption(res)) {
                                    | None => set_user_uusd_balance(_ => Some(0))
                                    | Some(balance) => set_user_uusd_balance(_ => Some(BigNumber.to_number(balance)))
                                    };
                                    Js.Promise.resolve()
                                });
                                ()
                            }
                            };
                            Js.Promise.resolve()
                        })
                    }
                    };
                    Js.Promise.resolve()
                })
                None
            }
            }
        }
        }
    }, [|user_address|]);

        <>
            <Header level user_address user_xtz_balance user_uusd_balance selected_token />
            <CenterBox 
                user_address 
                set_user_address 
                wallet
                set_wallet
                set_selected_token
                selected_token
                amount_to_send
                set_amount_to_send
            />
            <footer></footer>
        </>
    ;
}