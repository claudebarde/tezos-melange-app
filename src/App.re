[@react.component]
let make = () => {
    let context = React.useContext(Context.context);
    
    let (level, set_level) = React.useState(() => None);
    let (wallet, set_wallet) = React.useState(() => None);

    React.useEffect1(() => {
        open Taquito;

        let tezos = tezos_toolkit("https://ghostnet.ecadinfra.com");
        let _ = context.set_tezos(_ => Some(tezos));
        let rpc = tezos |> TezosToolkit.rpc;
        let _ = rpc |> RpcClient.get_block_header |> Js.Promise.then_(res => {
            let _ = set_level(_ => Some(res.level));
            let _ = Js.Global.setInterval(() => {
                let _ = rpc 
                |> RpcClient.get_block_header 
                |> Js.Promise.then_(res => {
                    let _ = set_level(_ => Some(res.level));
                    Js.Promise.resolve();
                });
                ()
            }, 10_000);
            Js.Promise.resolve();
        })
        None
    }, [||]);

    // fetches the user's XTZ and uUSD balances if connected
    React.useEffect1(() => {
        open Taquito;

        switch context.user_address {
        | None => {
            let _ = context.set_selected_token(_ => NOTOKEN);
            None
        }
        | Some(address) => {
            let _ = context.set_selected_token(_ => XTZ);
            switch context.tezos {
            | None => None
            | Some(tezos) => {
                // fetches the XTZ balance
                let _ = 
                    tezos
                    |> TezosToolkit.tz_provider
                    |> TzProvider.get_balance(address)
                    |> Js.Promise.then_(res => {
                        let _ = switch (Js.Nullable.toOption(res)){
                            | None => context.set_xtz_balance(_ => Some(0))
                            | Some(balance) => context.set_xtz_balance(_ => Some(BigNumber.to_number(balance)))
                        };
                        Js.Promise.resolve()
                    });
                // fetches the uUSD balance
                let _ =
                tezos
                |> TezosToolkit.contract
                |> ContractProvider.at(Context.Utils.uusd_address)
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
                                // let key: Uusd.Ledger.key = { owner: address, token_id: 0};
                                let key: Uusd.Ledger.key = (address, 0);
                                let _ = 
                                storage
                                |>Uusd.ledger
                                |>Uusd.Ledger.get(key)
                                |> Js.Promise.then_(res => {
                                    let _ = switch(Js.Nullable.toOption(res)) {
                                    | None => context.set_uusd_balance(_ => Some(0))
                                    | Some(balance) => {
                                        context.set_uusd_balance(_ => Some(BigNumber.to_number(balance)))
                                    }
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
    }, [|context.user_address|]);

    {
        switch context.tezos {
            | None => React.null
            | Some(_) =>
                <>
                    <Header level />
                    <CenterBox set_wallet wallet />
                    <footer></footer>
                </>
        }
    }
}