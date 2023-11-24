type styles_css = {. 
  "box": string,
  "connect_wallet": string
};

[@mel.module] external styles: styles_css = "./styles/CenterBox.module.css";

[@react.component]
let make = () => {
  React.useEffect0(() => {
    open Taquito;

    let tezos = tezos_toolkit("https://mainnet.ecadinfra.com");
    let rpc = tezos->TezosToolkit.rpc;
    let _ = Js.log(rpc);
    None
  });

  <div className=styles##box> 
      <h1> {"Welcome to the Tezos Melange app"->React.string} </h1>
      <button className=styles##connect_wallet>{"Connect wallet"->React.string}</button>
  </div>
}