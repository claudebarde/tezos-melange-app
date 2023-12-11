[@react.component]
let make = (
        ~wallet: option(BeaconWallet.t),
        ~set_wallet: (option(BeaconWallet.t) => option(BeaconWallet.t)) => unit
    ) => {
    let context = React.useContext(Context.context);

    let connect_wallet = () => {
        switch(wallet) {
            | None => {
                let wallet_options = BeaconWallet.new_wallet_options(~name="Tezos Melange App", ());
                let new_wallet = BeaconWallet.new_wallet(wallet_options);
                set_wallet(_ => Some(new_wallet))
                let _ = 
                    new_wallet 
                    |> BeaconWallet.request_permissions
                    |> Js.Promise.then_(_ => {
                        let _ = 
                            new_wallet 
                            |> BeaconWallet.get_pkh 
                            |> Js.Promise.then_(address => {
                                let _ = context.set_user_address(_ => Some(address));
                                Js.Promise.resolve()
                            })
                        Js.Promise.resolve()
                    })
                    |> Js.Promise.catch(err => {
                        let _ = Js.log(err);
                        Js.Promise.reject(Js.Exn.raiseError("Error while requesting wallet permissions"))
                    });
            }
            | _ => ()
        }
    }

    let disconnect_wallet = () => {
        switch wallet {
            | None => ()
            | Some(wallet) => 
                let _ = wallet.client 
                |> BeaconWallet.clear_active_account 
                |> Js.Promise.then_(_ => {
                    let _ = set_wallet(_ => None);
                    let _ = context.set_user_address(_ => None);
                    Js.Promise.resolve()
                });
                ()
        }
    }

    React.useEffect1(() => {
        let wallet_options = BeaconWallet.new_wallet_options(~name="Tezos Melange App", ());
        let new_wallet = BeaconWallet.new_wallet(wallet_options);
        let _ = new_wallet.client 
        |> BeaconWallet.get_active_account 
        |> Js.Promise.then_(res => {
            switch(Js.Nullable.toOption(res)){
                | None => {
                    let _ = set_wallet(_ => None);
                    let _ = context.set_user_address(_ => None);
                    Js.Promise.resolve()
                }
                | Some(active_account) => {
                    let _ = set_wallet(_ => Some(new_wallet));
                    let _ = context.set_user_address(_ => Some(active_account##address));
                    Js.Promise.resolve()
                }
            }
        });

        None
    }, [||]);

    {
        switch context.user_address {
            | None => 
                <button  
                    onClick={_ => connect_wallet ()}
                >
                    {"Connect wallet"->React.string}
                </button>
            | Some(_) =>
                <button 
                    onClick={_ => disconnect_wallet ()}
                >
                    {"Disconnect wallet"->React.string}
                </button>
        }
    }
}