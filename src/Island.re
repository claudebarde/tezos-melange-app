type styles_css = {. 
  "body": string,
  "cross": string,
  "check": string,
  "left": string,
  "center": string,
  "right": string,
  "small": string,
  "wide": string,
  "pulsate": string
};

type image = {. 
  "default": string
};

[@mel.module] external styles: styles_css = "./styles/Island.module.css";

[@mel.module "./assets/wallet-90.png"] external wallet_logo: string = "default";
[@mel.module "./assets/cross-90.png"] external cross_logo: string = "default";
[@mel.module "./assets/check-90.png"] external check_logo: string = "default";
[@mel.module "./assets/send-90.png"] external send_logo: string = "default";
[@mel.module "./assets/keyboard-90.png"] external keyboard_logo: string = "default";
[@mel.module "./assets/error-90.png"] external error_logo: string = "default";

[@react.component]
let make = (
    ~center_text: option(string), 
    ~connected: bool, 
    ~right_cell: Utils.right_cell_status,
    ~update_right_cell: (Utils.right_cell_status => Utils.right_cell_status) => unit,
    ~selected_token: Context.selected_token,
    ~show_selected_token: Context.selected_token => string) => {

    let dummy_timeout_id = Js.Global.setTimeout(() => (), 1);

    let (_, set_typing_timeout) = React.useState(() => dummy_timeout_id);

    // let _ = 
    //     switch context.amount_to_send {
    //         | None => set_typing_amount(_ => false)
    //         | Some(_) => {
    //             let _ = set_typing_amount(_ => true);
    //             let _ = Js.Global.setTimeout(() => set_typing_amount(_ => false), 1000);
    //             ()
    //         }
    //     };

    React.useEffect1(() => {
        let _ = 
            switch right_cell {
                | Typing => {
                    set_typing_timeout(prev_timeout => {
                        let timeout = 3_000;
                        let _ = Js.Global.clearTimeout(prev_timeout);
                        Js.Global.setTimeout(() => {
                            let _ = set_typing_timeout(_ => dummy_timeout_id);
                            update_right_cell(_ => Utils.Send)
                        }, timeout)
                        // if (prev_timeout == 0) {
                        //     // setting up the first timeout
                        //     Js.Global.setTimeout(() => set_typing_timeout(_ => dummy_timeout_id), timeout)
                        // }
                        // else {
                        //     // timeout already exists, must be canceled before setting up a new one
                        //     let _ = Js.Global.clearTimeout(prev_timeout);
                        //     Js.Global.setTimeout(() => set_typing_timeout(_ => dummy_timeout_id), timeout)
                        // }
                    })
                }
                | _ => set_typing_timeout(_ => dummy_timeout_id)
            };

        None
    }, [|right_cell|]);

    <div className={styles##body ++ " " ++ (connected ? styles##wide : styles##small)}>
        <div className=styles##left>
            {
                (if (connected == true) {
                    <>
                        <img src=wallet_logo alt="wallet" />
                        <img src=check_logo alt="wallet" className=styles##check />
                    </>
                } else {
                    <>
                        <img src=wallet_logo alt="wallet" />
                        <img src=cross_logo alt="wallet" className=styles##cross />
                    </>
                })
            }
        </div>
        <div className=styles##center>{(
            switch center_text {
                | None => "No wallet"
                | Some(text) => text
            }
        )->React.string}</div>
        <div className=styles##right>
            {
                (switch right_cell {
                    | NoWallet => React.null
                    | Send =>
                        switch selected_token {
                            | NOTOKEN => React.null
                            | _ => 
                                <>
                                    <img src=send_logo alt="send" />
                                    {show_selected_token(selected_token)}->React.string
                                </>
                        }
                    | Sending => React.null
                    | Error => <img src=error_logo alt="error" />
                    | Typing => <img className=styles##pulsate src=keyboard_logo alt="keyboard" />
                })
            }
        </div>
    </div>
}