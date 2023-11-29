let show_user_address = (address: string): string => {
    "Your address: " 
    ++ Js.String.slice(~from=0, ~to_=7, address) 
    ++ "..." 
    ++ Js.String.sliceToEnd(~from=-7, address)
};

[@react.component]
let make = (~level, ~user_address, ~user_xtz_balance) =>
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
      <div>
        {
          switch user_xtz_balance {
            | None => "Loading"
            | Some(balance) => 
              "XTZ: " ++ (((balance |> Js.Float.fromString) /. 1_000_000.0) |> Js.Float.toString)
          }
        }
        -> React.string
      </div>
    </header>