type styles_css = {. 
  "box": string,
  "select_token": string,
  "select_token_radio": string,
  "buttons": string
};

[@mel.module] external styles: styles_css = "./styles/CenterBox.module.css";

[@react.component]
let make = (~wallet, ~set_wallet) => {
  let context = React.useContext(Context.context);

  let (error_amount, set_error_amount) = React.useState(_ => false);

  <div className=styles##box> 
      <h1> {"Welcome to the Tezos Melange app"->React.string} </h1>
      {
        switch context.user_address {
          | None => <WalletButton wallet set_wallet />
          | Some(_) =>
            <div className=styles##select_token>
              <div className=styles##select_token_radio>
                <label htmlFor="select-xtz">
                  <input 
                    type_="radio" 
                    name="action-select" 
                    value={context.show_selected_token(XTZ)} 
                    checked={context.selected_token == XTZ}
                    onChange=(_ => context.set_selected_token(_ => XTZ))
                  />
                  <span>{"Send XTZ"->React.string}</span>
                </label>
                <label htmlFor="select-uusd">
                  <input 
                    type_="radio" 
                    name="action-select" 
                    value={context.show_selected_token(UUSD)}  
                    checked={context.selected_token == UUSD}
                    onChange=(_ => context.set_selected_token(_ => UUSD))
                  />
                  <span>{"Send uUSD"->React.string}</span>
                </label>
              </div>
              <input 
                type_="text" 
                className={error_amount == true ? "error" : ""}
                placeholder={context.show_selected_token(context.selected_token)} 
                onChange=(event => {
                  let _ = context.set_island_right_cell_status(_ => Utils.Typing);
                  // resets error display
                  let _ = set_error_amount(_ => false);
                  // checks if error
                  let value = ReactEvent.Form.target(event)##value;
                  let _ = 
                    switch value->Belt.Float.fromString {
                      | None => {
                        if (value->Belt.Float.toString->Js.String.length == 0) {
                            set_error_amount(_ => false)
                          } 
                          else {
                            let _ = context.set_island_right_cell_status(_ => Utils.Error);
                            set_error_amount(_ => true)
                          }
                      }
                      | Some(float_val) => {
                          if (float_val->Belt.Float.toString != value) {
                            let _ = context.set_island_right_cell_status(_ => Utils.Error);
                            set_error_amount(_ => true)
                          } 
                          else {
                            set_error_amount(_ => false)
                          }
                        }
                    };
                  context.set_amount_to_send(_ => value)
                })
                value={
                  switch context.amount_to_send {
                    | None => ""
                    | Some(value) => value->Belt.Float.toString
                  }
                }
              />
              <div className=styles##buttons>
                <button>{("Send " ++ context.show_selected_token(context.selected_token))->React.string}</button>
                <WalletButton wallet set_wallet />
              </div>
            </div>
        }
      }
  </div>
}