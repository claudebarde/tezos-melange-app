type styles_css = {. 
  "body": string,
  "cross": string,
  "check": string,
  "left": string,
  "center": string,
  "right": string,
  "small": string,
  "wide": string,
  "pulsate": string,
  "error": string,
  "hourglass": string
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
[@mel.module "./assets/hourglass-90.png"] external hourglass_logo: string = "default";
[@mel.module "./assets/thumb-up-90.png"] external thumb_up_logo: string = "default";

[@react.component]
let make = (
    ~center_text: option(string), 
    ~connected: bool, 
    ~right_cell: Context.Utils.right_cell_status,
    ~update_right_cell: (Context.Utils.right_cell_status => Context.Utils.right_cell_status) => unit,
    ~selected_token: Context.selected_token) => {

    let (typing_timeout, set_typing_timeout) = React.useState(() => None);

    React.useEffect1(() => {
        let _ = 
            switch right_cell {
                | Typing => {
                    set_typing_timeout(prev_timeout => {
                        let timeout = 3_000;
                        let _ = switch prev_timeout {
                            | Some(timeout) => {
                                let _ = Js.Global.clearTimeout(timeout);
                            }
                            | _ => ()
                        };
                        Js.Global.setTimeout(() => {
                            update_right_cell(_ => Context.Utils.Send)
                        }, timeout)->Some
                    })
                }
                | _ => 
                    switch typing_timeout {
                        | None => ()
                        | Some(timeout) => {
                            let _ = Js.Global.clearTimeout(timeout);
                            set_typing_timeout(_ => None)
                        }
                    }
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
                                    {Context.Utils.show_selected_token(selected_token)}->React.string
                                </>
                        }
                    | Sending => <img className=styles##hourglass src=hourglass_logo alt="hourglass" />
                    | Success => <img src=thumb_up_logo alt="error" />
                    | Error => <img className=styles##error src=error_logo alt="error" />
                    | Typing => <img className=styles##pulsate src=keyboard_logo alt="keyboard" />
                })
            }
        </div>
    </div>
}