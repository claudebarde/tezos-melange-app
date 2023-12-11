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


[@react.component]
let make = (~level) => {
  let context = React.useContext(Context.context);

  let balance_to_element = (balance: option(int), token: Context.selected_token): React.element => {
      <>
        <span>{(Context.Utils.show_selected_token(token) ++ ": ")->React.string}</span>
        <span>
          {
            (switch balance {
            | None => "Loading"
            | Some(balance) => {
              switch (Context.Utils.token_to_display(balance, token)) {
                  | Ok(res) => res
                  | Error(err) => err
                }
            }                
            })-> React.string
          }
        </span>
      </>
  };

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
          | None => 
              <Island 
                connected=false 
                center_text=None 
                right_cell=context.island_right_cell_status 
                update_right_cell=context.set_island_right_cell_status
                selected_token=context.selected_token
              />
          | Some(address) => 
              <Island 
                connected=true 
                center_text=(address->show_user_address->Some)  
                right_cell=context.island_right_cell_status 
                update_right_cell=context.set_island_right_cell_status
                selected_token=context.selected_token
              />
        })
      }
    </div>
    <div className=styles##tokens>
      {
        (switch(context.user_address) {
          | None => React.null
          | Some(_) => 
            <>
              <div>{ balance_to_element(context.user_xtz_balance, XTZ) }</div>
              <div>{ balance_to_element(context.user_uusd_balance, UUSD) }</div>
            </>
        })
      }
    </div>
  </header>
}