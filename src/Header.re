type styles_css = {. 
  "header": string,
  "tokens": string
};

[@mel.module] external styles: styles_css = "./styles/Header.module.css";

let show_user_address = (address: string): string => {
    "Your address: " 
    ++ Js.String.slice(~from=0, ~to_=7, address) 
    ++ "..." 
    ++ Js.String.sliceToEnd(~from=-7, address)
};

let balance_to_element = (balance: option(int), token: string): React.element => {
    <>
      <span>{(token ++ ": ")->React.string}</span>
      <span>
        {
          (switch balance {
          | None => "Loading"
          | Some(balance) => 
                {
                  (((
                      balance |> Js.Int.toFloat
                    ) /. 1_000_000.0
                    ) 
                    |> Js.Float.toString
                  )
                }
              
          })-> React.string
        }
      </span>
    </>
};

[@react.component]
let make = (~level, ~user_address, ~user_xtz_balance, ~user_uusd_balance) =>
    <header>
        <div>
        {
          (switch(level) {
            | None => "Loading"
            | Some(level) => "Block level " ++ (level->Belt.Int.toString)
          })
          ->React.string
        }
      </div>
      <div>
        {
          (switch(user_address) {
            | None => ""
            | Some(address) => show_user_address(address)
          })
          ->React.string
        }
      </div>
      <div className=styles##tokens>
        <div>{ balance_to_element(user_xtz_balance, "XTZ") }</div>
        <div>{ balance_to_element(user_uusd_balance, "uUSD") }</div>
      </div>
    </header>