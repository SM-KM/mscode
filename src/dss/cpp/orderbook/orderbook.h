#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <algorithm>
#include <cstdint>
#include <format>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace dss
{

enum class OrderType
{
  GoodTillCancel,
  FillAndKill
};

enum class Side
{
  Buy,
  Sell
};

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

struct LevelInfo
{
  Price price_;
  Quantity quantity_;
};

using levelInfos = std::vector<LevelInfo>;

class OrderbookLevelInfos
{
public:
  OrderbookLevelInfos(const levelInfos& bids, const levelInfos& asks)
      : bids_{bids}, asks_{asks}
  {
  }

  const levelInfos& get_bids() const { return bids_; }
  const levelInfos& get_asks() const { return asks_; }

private:
  levelInfos bids_;
  levelInfos asks_;
};

class order
{
public:
  order(OrderType order_type, OrderId order_id, Side side, Price price,
        Quantity quantity)
      : order_type_{order_type}, order_id_{order_id}, side_{side},
        price_{price}, initial_quantity_{quantity},
        remaining_quantity_{quantity}
  {
  }

  OrderId get_order_id() const { return order_id_; }
  Side get_side() const { return side_; }
  Price get_price() const { return price_; }
  OrderType get_order_type() const { return order_type_; }
  Quantity get_initial_quantity() const { return initial_quantity_; }
  Quantity get_remaining_quantity() const { return remaining_quantity_; }
  Quantity get_filled_quantity() const
  {
    return get_initial_quantity() - get_remaining_quantity();
  }

  bool is_filled() const { return get_remaining_quantity() == 0; }

  void Fill(Quantity quantity)
  {
    if (quantity > get_remaining_quantity())
    {
      throw std::logic_error(std::format(
          "Order ({}) cannot be filled for more than its remaining quantity",
          get_order_id()));
    }

    remaining_quantity_ -= quantity;
  }

private:
  OrderType order_type_;
  OrderId order_id_;
  Side side_;
  Price price_;
  Quantity initial_quantity_;
  Quantity remaining_quantity_;
};

using OrderPointer = std::shared_ptr<order>;
using OrderPointers = std::list<OrderPointer>;

class OrderModify
{
public:
  OrderModify(OrderId orderid, Side side, Price price, Quantity quantity)
      : orderid_{orderid}, price_{price}, side_{side}, quantity_{quantity}
  {
  }

  OrderId get_order_id() const { return orderid_; }
  Price get_price() const { return price_; }
  Side get_side() const { return side_; }
  Quantity get_quantity() const { return quantity_; }

  OrderPointer to_order_pointer(OrderType type) const
  {
    return std::make_shared<order>(type, get_order_id(), get_side(),
                                   get_price(), get_quantity());
  }

private:
  OrderId orderid_;
  Price price_;
  Side side_;
  Quantity quantity_;
};

struct TradeInfo
{
  OrderId orderid_;
  Price price_;
  Quantity quantity_;
};

class Trade
{
public:
  Trade(const TradeInfo& bid_trade, const TradeInfo& ask_trade)
      : bid_trade_{bid_trade}, ask_trade_{ask_trade}
  {
  }

  const TradeInfo& get_bid_trade() const { return bid_trade_; }
  const TradeInfo& get_ask_trade() const { return ask_trade_; }

private:
  TradeInfo bid_trade_;
  TradeInfo ask_trade_;
};

using Trades = std::vector<Trade>;

class Orderbook
{
private:
  struct OrderEntry
  {
    OrderPointer order_{nullptr};
    OrderPointers::iterator location_;
  };

  std::map<Price, OrderPointers, std::greater<Price>> bids_;
  std::map<Price, OrderPointers, std::less<Price>> asks_;
  std::unordered_map<OrderId, OrderEntry> orders_;

  bool can_match(Side side, Price price) const
  {
    if (side == Side::Buy)
    {
      if (asks_.empty()) return false;
      const auto& [best_ask, _] = *asks_.begin();
      return price >= best_ask;
    }
    else
    {
      if (bids_.empty()) return false;
      const auto& [best_bid_, _] = *bids_.begin();
      return price <= best_bid_;
    }
  }

  Trades match_orders()
  {
    Trades trades;
    trades.reserve(orders_.size());

    while (true)
    {
      if (bids_.empty() || asks_.empty()) break;
      auto& [bid_price, bids] = *bids_.begin();
      auto& [ask_price, asks] = *asks_.begin();

      if (bid_price < ask_price) break;
      while (bids.size() && asks.size())
      {
        auto& bid = bids.front();
        auto& ask = asks.front();

        Quantity quantity = std::min(bid->get_remaining_quantity(),
                                     ask->get_remaining_quantity());

        bid->Fill(quantity);
        ask->Fill(quantity);

        if (bid->is_filled())
        {
          bids.pop_front();
          orders_.erase(bid->get_order_id());
        }

        if (ask->is_filled())
        {
          asks.pop_front();
          orders_.erase(ask->get_order_id());
        }

        if (bids.empty()) bids_.erase(bid_price);
        if (asks.empty()) asks_.erase(ask_price);

        trades.push_back(
            Trade{TradeInfo{bid->get_order_id(), bid->get_price(), quantity},
                  TradeInfo{ask->get_order_id(), ask->get_price(), quantity}});
      }
    }

    if (!bids_.empty())
    {
      auto& [_, bids] = *bids_.begin();
      auto& order = bids.front();
      if (order->get_order_type() == OrderType::FillAndKill)
        cancel_order(order->get_order_id());
    }

    if (!asks_.empty())
    {
      auto& [_, asks] = *asks_.begin();
      auto& order = asks.front();
      if (order->get_order_type() == OrderType::FillAndKill)
        cancel_order(order->get_order_id());
    }

    return trades;
  }

public:
  Trades add_order(OrderPointer order)
  {
    if (orders_.contains(order->get_order_id())) return {};
    if (order->get_order_type() == OrderType::FillAndKill &&
        !can_match(order->get_side(), order->get_price()))
      return {};

    OrderPointers::iterator iterator;
    if (order->get_side() == Side::Buy)
    {
      auto& orders = bids_[order->get_price()];
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size() - 1);
    }
    else
    {
      auto& orders = asks_[order->get_price()];
      orders.push_back(order);
      iterator = std::next(orders.begin(), orders.size() - 1);
    }

    orders_.insert({order->get_order_id(), OrderEntry{order, iterator}});
    return match_orders();
  }

  void cancel_order(OrderId order_id)
  {
    if (!orders_.contains(order_id)) return;
    const auto& [order, iterator] = orders_.at(order_id);
    orders_.erase(order_id);

    if (order->get_side() == Side::Sell)
    {
      auto price = order->get_price();
      auto& orders = asks_.at(price);
      orders.erase(iterator);
      if (orders.empty()) asks_.erase(price);
    }
    else
    {
      auto price = order->get_price();
      auto& orders = bids_.at(price);
      orders.erase(iterator);
      if (orders.empty()) bids_.erase(price);
    }
  }

  Trades match_order(OrderModify order)
  {
    if (!orders_.contains(order.get_order_id())) return {};
    const auto& [existing_order, _] = orders_.at(order.get_order_id());
    cancel_order(order.get_order_id());
    return add_order(order.to_order_pointer(existing_order->get_order_type()));
  }

  std::size_t size() const { return orders_.size(); };

  OrderbookLevelInfos get_order_infos() const
  {
    levelInfos bid_infos, ask_infos;
    bid_infos.reserve(orders_.size());
    ask_infos.reserve(orders_.size());

    auto create_level_infos = [](Price price, const OrderPointers& orders)
    {
      return LevelInfo{
          price,
          std::accumulate(
              orders.begin(), orders.end(), (Quantity)0,
              [](std::size_t running_sum, const OrderPointer& order)
              { return running_sum + order->get_remaining_quantity(); })};
    };

    for (const auto& [price, orders] : bids_)
      bid_infos.push_back(create_level_infos(price, orders));
    for (const auto& [price, orders] : asks_)
      ask_infos.push_back(create_level_infos(price, orders));
    return OrderbookLevelInfos(bid_infos, ask_infos);
  }
};

} // namespace dss

#endif // ORDERBOOK_H
