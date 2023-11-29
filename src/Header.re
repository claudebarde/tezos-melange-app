let show_user_address = (address: string): string => {
    "Your address: " 
    ++ Js.String.slice(~from=0, ~to_=7, address) 
    ++ "..." 
    ++ Js.String.sliceToEnd(~from=-7, address)
};

[@react.component]
let make = (~level, ~user_address) =>
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
    </header>