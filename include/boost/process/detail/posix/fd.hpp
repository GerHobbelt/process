// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROCESS_DETAIL_POSIX_FD_HPP
#define BOOST_PROCESS_DETAIL_POSIX_FD_HPP

#include <boost/process/detail/posix/handler.hpp>
#include <unistd.h>

namespace boost { namespace process { namespace detail { namespace posix {


struct close_fd_ : handler_base_ext
{
    close_fd_(int fd) : fd_(fd) {}

    template <class PosixExecutor>
    void on_exec_setup(PosixExecutor&) const
    {
        ::close(fd_);
    }

private:
    int fd_;
};

template <class Range>
struct close_fds_ : handler_base_ext
{
public:
    close_fds_(const Range &fds) : fds_(fds) {}

    template <class PosixExecutor>
    void on_exec_setup(PosixExecutor&) const
    {
        for (auto & fd_ : fds_)
            ::close(fd_);
    }

private:
    Range fds_;
};



template <class FileDescriptor>
struct bind_fd_ : handler_base_ext
{
public:
    bind_fd_(int id, const FileDescriptor &fd) : id_(id), fd_(fd) {}

    template <class PosixExecutor>
    void on_exec_setup(PosixExecutor&) const
    {
        ::dup2(fd_.handle(), id_);
    }

private:
    int id_;
    FileDescriptor fd_;
};


struct fd_
{
	constexpr fd_() {};
	close_fd_ close(int _fd) const {return close_fd_(_fd);}
	close_fds_<std::vector<int>> close(const std::initializer_list<int> & vec) const {return std::vector<int>(vec);}
	template<typename Range>
	close_fds_<Range> close(const Range & r) const {return r;}

	template <class FileDescriptor>
	bind_fd_<FileDescriptor> bind(int id, const FileDescriptor & fd) const {return {id, fd};}

};

constexpr static fd_ fd;

}}}}

#endif