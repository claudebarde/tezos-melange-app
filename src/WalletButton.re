type styles_css = {. 
  "connect_wallet": string
};

[@mel.module] external styles: styles_css = "./styles/WalletButton.module.css";

[@react.component]
let make = (
        ~user_address: option(string), 
        ~set_user_address: (option(string) => option(string)) => unit
    ) => {
    let (wallet, set_wallet) = React.useState(() => None);

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
                                let _ = set_user_address(_ => Some(address));
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
                    let _ = set_user_address(_ => None);
                    Js.Promise.resolve()
                });
                ()
        }
    }

    React.useEffect0(() => {
        let wallet_options = BeaconWallet.new_wallet_options(~name="Tezos Melange App", ());
        let new_wallet = BeaconWallet.new_wallet(wallet_options);
        let _ = new_wallet.client 
        |> BeaconWallet.get_active_account 
        |> Js.Promise.then_(res => {
            switch(Js.Nullable.toOption(res)){
                | None => {
                    let _ = set_wallet(_ => None);
                    let _ = set_user_address(_ => None);
                    Js.Promise.resolve()
                }
                | Some(active_account) => {
                    let _ = Js.log(active_account);
                    let _ = set_wallet(_ => Some(new_wallet));
                    let _ = set_user_address(_ => Some(active_account##address));
                    Js.Promise.resolve()
                }
            }
        });

        None
    });

    {
        switch user_address {
            | None => 
                <button 
                    className=styles##connect_wallet 
                    onClick={_ => connect_wallet ()}
                >
                    {"Connect wallet"->React.string}
                </button>
            | Some(_) =>
                <button 
                    className=styles##connect_wallet
                    onClick={_ => disconnect_wallet ()}
                >
                    {"Disconnect wallet"->React.string}
                </button>
        }
    }
}