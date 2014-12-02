// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#ifndef CEPH_TEST_RADOS_CLIENT_H
#define CEPH_TEST_RADOS_CLIENT_H

#include "include/rados/librados.hpp"
#include "common/config.h"
#include "include/atomic.h"
#include "include/buffer.h"
#include "test/librados_test_stub/TestWatchNotify.h"
#include <boost/function.hpp>
#include <map>
#include <string>
#include <vector>

class Finisher;

namespace librados {

class TestIoCtxImpl;

class TestRadosClient {
public:

  typedef boost::function<int()> AioFunction;

  TestRadosClient(CephContext *cct);

  void get();
  void put();

  virtual CephContext *cct();

  virtual uint64_t get_instance_id();

  virtual int connect();
  virtual void shutdown();
  virtual int wait_for_latest_osdmap();

  virtual TestIoCtxImpl *create_ioctx(int64_t pool_id,
                                      const std::string &pool_name) = 0;

  virtual int mon_command(const std::vector<std::string>& cmd,
                          const bufferlist &inbl,
                          bufferlist *outbl, std::string *outs);

  virtual int pool_create(const std::string &pool_name) = 0;
  virtual int pool_delete(const std::string &pool_name) = 0;
  virtual int pool_get_base_tier(int64_t pool_id, int64_t* base_tier) = 0;
  virtual int pool_list(std::list<std::string> *v) = 0;
  virtual int64_t pool_lookup(const std::string &name) = 0;
  virtual int pool_reverse_lookup(int64_t id, std::string *name) = 0;

  TestWatchNotify &get_watch_notify() {
    return m_watch_notify;
  }

  void add_aio_operation(const AioFunction &aio_function, AioCompletionImpl *c);
  void flush_aio_operations();

protected:
  virtual ~TestRadosClient();

private:

  CephContext *m_cct;
  atomic_t m_refcount;

  Finisher *m_finisher;
  TestWatchNotify m_watch_notify;

};

} // namespace librados

#endif // CEPH_TEST_RADOS_CLIENT_H
