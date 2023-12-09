type styles_css = {. 
  "body": string,
  "cross": string,
  "check": string,
  "left": string,
  "center": string,
  "right": string,
  "small": string,
  "wide": string
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

[@react.component]
let make = (~center_text: option(string), ~connected: bool) => {
    let context = React.useContext(Context.context);

    // let (typing_amount, set_typing_amount) = React.useState(() => false);

    // let _ = 
    //     switch context.amount_to_send {
    //         | None => set_typing_amount(_ => false)
    //         | Some(_) => {
    //             let _ = set_typing_amount(_ => true);
    //             let _ = Js.Global.setTimeout(() => set_typing_amount(_ => false), 1000);
    //             ()
    //         }
    //     };

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
            // {
            //     typing_amount ? 
            //         <img src=keyboard_logo alt="keyboard" /> : 
            //         <img src=send_logo alt="send" />
            // }
            {
                (switch context.user_address {
                    | None => React.null
                    | Some(_) => 
                        <>
                            <img src=send_logo alt="send" />
                            {Utils.show_selected_token(context.selected_token)}->React.string
                        </>
                })
            }
        </div>
    </div>
}