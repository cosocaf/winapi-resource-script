import { ExtensionContext } from "vscode";
import { ServerOptions, LanguageClientOptions, LanguageClient } from "vscode-languageclient/node";

let client: LanguageClient;

export function activate(context: ExtensionContext) {
  const serverOptions: ServerOptions = {
    command: context.asAbsolutePath("../language-server/build/src/Debug/rc-language-server.exe")
  };
  const clientOptions: LanguageClientOptions = {
    documentSelector: [{ scheme: "file", language: "rc" }]
  };
  client = new LanguageClient("WinAPIResourceScript", "WinAPI Resource Script", serverOptions, clientOptions, true);
  context.subscriptions.push(client.start());
}

export function deactive() {
  if (client) {
    return client.stop();
  }
  return undefined;
}