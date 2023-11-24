type styles_css = {. 
  "connect_wallet": string
};

[@mel.module] external styles: styles_css = "./styles/WalletButton.module.css";

[@react.component]
let make = () => {
    let (wallet, set_wallet) = React.useState(() => None);

    <button 
        className=styles##connect_wallet 
        onClick={_ => {
            open Taquito

            switch(wallet) {
                | None => {
                    // let wallet_options: BeaconWallet.new_wallet_options = { name: "Tezos Melange App" };
                    let wallet_options = BeaconWallet.new_wallet_options(~name="Tezos Melange App", ());
                    let new_wallet = BeaconWallet.new_wallet(wallet_options);
                    set_wallet(_ => Some(new_wallet))
                }
                | _ => ()
            }
        }}
    >
        {"Connect wallet"->React.string}
    </button>
}