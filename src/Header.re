type styles_css = {. 
  "header": string,
  "tokens": string,
  "level": string,
  "center": string
};

[@mel.module] external styles: styles_css = "./styles/Header.module.css";

let show_user_address = (address: string): string => {
    Js.String.slice(~from=0, ~to_=7, address) 
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
let make = (~level) => {
  let context = React.useContext(Context.context);

  <header>
      <div className=styles##level>
      {
        (switch(level) {
          | None => "Loading"
          | Some(level) => "Block level " ++ (level->Belt.Int.toString)
        })
        ->React.string
      }
    </div>
    <div className=styles##center>
      {
        (switch(context.user_address) {
          | None => <Island connected=false center_text=None />
          | Some(address) => <Island connected=true center_text=(address->show_user_address->Some) />
        })
      }
    </div>
    <div className=styles##tokens>
      {
        (switch(context.user_address) {
          | None => React.null
          | Some(_) => 
            <>
              <div>{ balance_to_element(context.user_xtz_balance, "XTZ") }</div>
              <div>{ balance_to_element(context.user_uusd_balance, "uUSD") }</div>
            </>
        })
      }
    </div>
  </header>
}