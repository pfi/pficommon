============================
pficommon: 汎用C++ライブラリ
============================

概要
====

C++用汎用ライブラリです。GCC 4.8.5 以降対応。

インストール
============

次の手順でインストールできます。

..

  $ ./waf configure
  $ ./waf build
  $ ./waf install


Tips
====
  - ./waf install は 権限が足らずに失敗する場合がある。　その際は、su なり sudo なりで 必要な権限を与えること。
  
  - pkg-config のパスが通ってなくて、pficommonを使用する プロジェクトの .configure が通らない事がある
  - その時には、"export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig" (例) で 環境変数を設定すること。 
  - ↑は CentOS 7 での例。 他の環境では path が異なる場合がある。 環境に合った pkgconfig ディレクトリの位置を指定すること。
