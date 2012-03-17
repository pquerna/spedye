# spedye: A reverse proxy for the HTTPS and SPDY protocols

Spedye is meant to handle all TLS/SSL traffic for a website -- it is based upon the ideas in [Bump's Stud](https://github.com/bumptech/stud), but extended to include converting SPDY connections into normal HTTP requests.

Spedye aims to make adopting the SPDY protocol extremely easy for all kinds of websites.

# STATUS: incomplete, prototype, work in progress, non-functional, etc, etc


## Goals / Features

* Built on top of [spdylay](https://github.com/tatsuhiro-t/spdylay).
* SPDY version 2 and version 3
* Multiple threads or processes each running their own libuv event loop to parallelize cryptographic operations across CPUs.
* Server Name Indication: Support for multiple Certificates on a single listening address.

## Usage

The simplest configuration is to forward all traffic on port `:443` to port `:80`:

    spedye -b 127.0.0.1:80 -f 0.0.0.0:443 -c /etc/ssl/mycert.crt -k /etc/ssl/mycert.key  

Spedye by default adds the following headers to the backend HTTP request:

* `X-Forwarded-For`: Client IPv4 or IPv6 address.
* `X-Forwarded-Proto`: Client protocol, always `https`
* `X-Forwarded-Spdy`: `true` or `false` if SPDY was used.
* `X-Forwarded-Host`: Set to the Common Name (CN) of the certificate used.

Spedye also supports using a configuration file for all options.

## License

Apache License 2.0, see the `LICENSE` file for details.



