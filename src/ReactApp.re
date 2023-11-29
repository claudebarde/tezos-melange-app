[%%raw "import './index.css'"];
// [@module] external styles: Js.t({..}) = "./ReactApp.module.css";

module App = {
  [@react.component]
  let make = () => {
    let (tezos, set_tezos) = React.useState(() => None)
    let (level, set_level) = React.useState(() => None);
    let (wallet, set_wallet) = React.useState(() => None);
    let (user_address, set_user_address) = React.useState(() => None);
    let (user_xtz_balance, set_user_xtz_balance) = React.useState(() => None);

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

    // fetches the user's balance if connected
    React.useEffect1(() => {
      open Taquito;

      switch user_address {
        | None => None
        | Some(address) => {
          switch tezos {
            | None => None
            | Some(tezos) => {
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
              None
            }
          }
        }
      }
    }, [|user_address|]);
    
      <>
        <Header level user_address user_xtz_balance />
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
