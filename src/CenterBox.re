type styles_css = {. 
  "box": string,
  "select_token": string,
  "select_token_radio": string,
  "buttons": string
};

[@mel.module] external styles: styles_css = "./styles/CenterBox.module.css";

type island_status = Context.Utils.right_cell_status;

[@react.component]
let make = (~wallet, ~set_wallet) => {
  let context = React.useContext(Context.context);
  let show_selected_token = Context.Utils.(show_selected_token);

  let (error_amount, set_error_amount) = React.useState(_ => false);
  let (recipient, set_recipient) = React.useState(_ => None);
  let (recipient_error, set_recipient_error) = React.useState(_ => false);
  let (is_transfer_disabled, set_transfer_disabled) = React.useState(_ => true);

  let transfer_xtz = () => {
    open Taquito;

    let _ = context.set_island_right_cell_status(_ => Sending);
    switch (context.tezos, context.amount_to_send, recipient, context.user_address) {
      // checks if the TezosToolkit has been intialized
      | (None, _, _, _) => Js.Console.error("TezosToolkit hasn't been instantiated")
      | (Some(tezos), Some(amount), Some(recipient), Some(user_address)) => {
        // builds the transfer parameters
        let transfer_param: BeaconWallet.transfer_param = { to_: recipient, amount: amount };
        // sends the transfer of XTZ
        let _ = 
          tezos
          |> TezosToolkit.wallet
          |> BeaconWallet.transfer(transfer_param)
          |> BeaconWallet.WalletSend.send
          |> Js.Promise.then_(op => {
            // if the operation is successfully sent
            let _ = Js.log(op |> BeaconWallet.WalletOperation.hash);
            // waits for 1 confirmation
            let _ = 
              op
              |> BeaconWallet.WalletOperation.confirmation(~confirmations=1, ~timeout=15000)
              |> Js.Promise.then_(res => {
                // if the operation is confirmed
                let _ = Js.log(res);
                let _ = context.set_island_right_cell_status(_ => Success);
                let _ = Js.Global.setTimeout(() => context.set_island_right_cell_status(_ => Send), 3_000);
                let _ = (context.set_amount_to_send(_ => None), set_recipient(_ => None));
                // fetches the new XTZ balance
                let _ = 
                  tezos
                  |> TezosToolkit.tz_provider
                  |> TzProvider.get_balance(user_address)
                  |> Js.Promise.then_(res => {
                      // updated the balance
                      let _ = switch (Js.Nullable.toOption(res)){
                          | None => context.set_xtz_balance(_ => Some(0))
                          | Some(balance) => context.set_xtz_balance(_ => Some(BigNumber.to_number(balance)))
                      };
                      Js.Promise.resolve()
                    })
                    |> Js.Promise.catch(err => {
                      let _ = Js.Console.error(err);
                      context.set_xtz_balance(_ => None)
                      Js.Promise.resolve()
                    });
                Js.Promise.resolve()
              })
              |> Js.Promise.catch(err => {
                // if there was a problem sending the operation
                let _ = Js.Console.error(err);
                let _ = context.set_island_right_cell_status(_ => Error);
                Js.Promise.resolve()
              });
            Js.Promise.resolve()
          })
          |> Js.Promise.catch(err => {
            let _ = Js.Console.error(err);
            let _ = context.set_island_right_cell_status(_ => Error);
            Js.Promise.resolve()
          })
      }
      | _ => Js.Console.error("An error has occured while initiating the transfer")
    }
  };

  React.useEffect2(() => {
    let disabled_transfer = 
      switch (context.amount_to_send, recipient) {
        // TODO: check the amount is valid
        | (Some(amount), Some(recipient)) => 
          !(amount > 0.) 
          || Js.String.length(recipient) < 1
          || error_amount 
          || recipient_error
        | _ => true
      };
    let _ = set_transfer_disabled(_ => disabled_transfer);

    None
  }, (context.amount_to_send, recipient));

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
                    value={show_selected_token(XTZ)} 
                    checked={context.selected_token == XTZ}
                    onChange=(_ => {
                      let _ = (
                        context.set_amount_to_send(_ => None), 
                        set_error_amount(_ => false),
                        context.set_island_right_cell_status(_ => Send)
                      );
                      context.set_selected_token(_ => XTZ)
                    })
                  />
                  <span>{"Send XTZ"->React.string}</span>
                </label>
                <label htmlFor="select-uusd">
                  <input 
                    type_="radio" 
                    name="action-select" 
                    value={show_selected_token(UUSD)}  
                    checked={context.selected_token == UUSD}
                    onChange=(_ => {
                      let _ = (
                        context.set_amount_to_send(_ => None), 
                        set_error_amount(_ => false),
                        context.set_island_right_cell_status(_ => Send)
                      );
                      context.set_selected_token(_ => UUSD)
                    })
                  />
                  <span>{"Send uUSD"->React.string}</span>
                </label>
              </div>
              <input 
                type_="text" 
                className={error_amount == true ? "error" : ""}
                placeholder={show_selected_token(context.selected_token)} 
                onChange=(event => {
                  let _ = context.set_island_right_cell_status(_ => Typing);
                  // resets error display
                  let _ = set_error_amount(_ => false);
                  let value = ReactEvent.Form.target(event)##value;
                  // checks if amount is not a number
                  let _ = 
                    switch value->Belt.Float.fromString {
                      | None => {
                        if (value->Belt.Float.toString->Js.String.length == 0) {
                            set_error_amount(_ => false)
                          } 
                          else {
                            let _ = context.set_island_right_cell_status(_ => Error);
                            set_error_amount(_ => true)
                          }
                      }
                      | Some(float_val) => {
                          if (float_val->Belt.Float.toString != value) {
                            let _ = context.set_island_right_cell_status(_ => Error);
                            set_error_amount(_ => true)
                          } 
                          else {
                            // checks if user has enough balance
                            switch context.selected_token {
                              | XTZ => {
                                switch ((Context.Utils.token_from_display(value, XTZ)), context.user_xtz_balance) {
                                  | (Ok(value), Some(balance)) => {
                                    // compares value with XTZ balance
                                    if (value > (balance |> Belt.Int.toFloat)) {
                                      let _ = context.set_island_right_cell_status(_ => Error);
                                      set_error_amount(_ => true);
                                    } else {
                                      set_error_amount(_ => false)
                                    }
                                  }
                                  | (Error(err), _) => {
                                    let _ = context.set_island_right_cell_status(_ => Error);
                                    let _ = set_error_amount(_ => true);
                                    Js.log(err)
                                  }
                                  | _ => {
                                    let _ = context.set_island_right_cell_status(_ => Error);
                                    let _ = set_error_amount(_ => true);
                                    Js.log("Unexpected error while comparing input amount with current XTZ balance")
                                  }
                                }
                              }
                              | UUSD => {
                                switch ((Context.Utils.token_from_display(value, UUSD)), context.user_uusd_balance) {
                                  | (Ok(value), Some(balance)) => {
                                    // compares value with uUSD balance
                                    if (value > (balance |> Belt.Int.toFloat)) {
                                      let _ = context.set_island_right_cell_status(_ => Error);
                                      set_error_amount(_ => true);
                                    } else {
                                      set_error_amount(_ => false)
                                    }
                                  }
                                  | (Error(err), _) => {
                                    let _ = context.set_island_right_cell_status(_ => Error);
                                    let _ = set_error_amount(_ => true);
                                    Js.log(err)
                                  }
                                  | _ => {
                                    let _ = context.set_island_right_cell_status(_ => Error);
                                    let _ = set_error_amount(_ => true);
                                    Js.log("Unexpected error while comparing input amount with current uUSD balance")
                                  }
                                }
                              }
                              | _ => ()
                            }
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
              <input
                type_="text"
                placeholder="Recipient"
                className={recipient_error == true ? "error" : ""}
                value={
                  switch recipient {
                    | None => ""
                    | Some(value) => value
                  }
                }
                onChange={event => {
                  let value = ReactEvent.Form.target(event)##value;
                  let _ = set_recipient(_ => value);
                  if (Js.String.length(value) > 0) {
                    switch (Taquito.Utils.validate_address(value)) {
                      | Ok() => {
                        let _ = set_recipient_error(_ => false);
                        context.set_island_right_cell_status(_ => Send)
                      }
                      | Error(err) => {
                        let _ = Js.log(err);
                        let _ = set_recipient_error(_ => true);
                        context.set_island_right_cell_status(_ => Error)
                      }
                    }
                  } else {
                    let _ = set_recipient_error(_ => false);
                    context.set_island_right_cell_status(_ => Send)
                  }
                }}
              />
              <div className=styles##buttons>
                <button
                  disabled={is_transfer_disabled}
                  onClick=(_ => transfer_xtz())
                >
                  {("Send " ++ show_selected_token(context.selected_token))->React.string}
                </button>
                <WalletButton wallet set_wallet />
              </div>
            </div>
        }
      }
  </div>
}