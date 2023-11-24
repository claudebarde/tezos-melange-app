type styles_css = {. 
  "box": string,
  "connect_wallet": string
};

[@mel.module] external styles: styles_css = "./styles/CenterBox.module.css";

[@react.component]
let make = () => {
  let (level, set_level) = React.useState(() => None);

  React.useEffect0(() => {
    open Taquito;

    let tezos = tezos_toolkit("https://mainnet.ecadinfra.com");
    let rpc = tezos |> TezosToolkit.rpc;
    // let block_header = rpc->RpcClient.get_block_header;
    let _ = rpc |> RpcClient.get_block_header |> Js.Promise.then_(res => {
      let _ = Js.log(res);
      let _ = set_level(_ => Some(res.level));
      Js.Promise.resolve();
    })
    None
  });

  <div className=styles##box> 
      <h1> {"Welcome to the Tezos Melange app"->React.string} </h1>
      <div>
        {
          (switch(level) {
            | None => "Loading"
            | Some(level) => "Level " ++ (level->Belt.Int.toString)
          })
          ->React.string
        }
      </div>
      <button className=styles##connect_wallet>{"Connect wallet"->React.string}</button>
  </div>
}