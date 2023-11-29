[%%raw "import './index.css'"];
// [@module] external styles: Js.t({..}) = "./ReactApp.module.css";

module App = {
  [@react.component]
  let make = () => {
    let (_tezos, set_tezos) = React.useState(() => None)
    let (level, set_level) = React.useState(() => None);
    let (user_address, set_user_address) = React.useState(() => None);
    let (wallet, set_wallet) = React.useState(() => None);

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
    
      <>
        <Header level user_address />
        <CenterBox 
          user_address 
          set_user_address 
          wallet
          set_wallet
        />
        <footer></footer>
      </>
    ;
  }
};

ReactDOM.querySelector("#root")
->(
    fun
    | Some(root) => ReactDOM.render(<App />, root)
    | None =>
      Js.Console.error(
        "Failed to start React: couldn't find the #root element",
      )
  );
