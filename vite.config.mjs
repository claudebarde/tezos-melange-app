import { fileURLToPath } from "url";
import path from "path";
import { defineConfig } from "vite";
import { nodeResolve } from '@rollup/plugin-node-resolve';
import react from "@vitejs/plugin-react";

const __filename = fileURLToPath(import.meta.url);
// this is the dir where files will be copied files will be copied by melange
const root = path.resolve(path.dirname(__filename), 'output');

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    nodeResolve(),
    react(),
  ],
  clearScreen: false,
  root,
  resolve: {
    alias: {
      src: path.resolve(root, "src"),
    },
  },
  build: {
    rollupOptions: {
      input: {
        main: path.resolve(root, "index.html"),
      },
    },
  },
});
