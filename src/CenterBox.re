type styles_css = {. 
  "box": string,
  "select_token": string,
  "select_token_radio": string,
  "buttons": string
};

type selected_token =
| XTZ
| UUSD;

let show_selected_token = (tk: selected_token): string => 
  switch tk {
    | XTZ => "XTZ"
    | UUSD => "uUSD"
  };

[@mel.module] external styles: styles_css = "./styles/CenterBox.module.css";

[@react.component]
let make = (~user_address, ~set_user_address, ~wallet, ~set_wallet) => {
  let (selected_token, set_selected_token) = React.useState(() => XTZ);

  <div className=styles##box> 
      <h1> {"Welcome to the Tezos Melange app"->React.string} </h1>
      {
        switch user_address {
          | None => <WalletButton user_address set_user_address wallet set_wallet />
          | Some(_) =>
            <div className=styles##select_token>
              <div className=styles##select_token_radio>
                <label htmlFor="select-xtz">
                  <input 
                    type_="radio" 
                    name="action-select" 
                    value={show_selected_token(XTZ)} 
                    checked={selected_token == XTZ}
                    onChange=(_ => set_selected_token(_ => XTZ))
                  />
                  <span>{"Send XTZ"->React.string}</span>
                </label>
                <label htmlFor="select-uusd">
                  <input 
                    type_="radio" 
                    name="action-select" 
                    value={show_selected_token(UUSD)}  
                    checked={selected_token == UUSD}
                    onChange=(_ => set_selected_token(_ => UUSD))
                  />
                  <span>{"Send uUSD"->React.string}</span>
                </label>
              </div>
              <input type_="text" placeholder={show_selected_token(selected_token)} />
              <div className=styles##buttons>
                <button>{("Send " ++ show_selected_token(selected_token))->React.string}</button>
                <WalletButton user_address set_user_address wallet set_wallet />
              </div>
            </div>
        }
      }
  </div>
}