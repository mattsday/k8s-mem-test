# Memory Limits vs Requests

This is a simple demo to show what happens if you set different requests and limits for memory and the scheduler needs to place a workload.

It works by using a [small C programme](image/memtest.c) that allocates the specified memory amount. You run through the following steps:

1. Create a single-node cluster with 4GiB RAM
2. Deploy the app with a request of 1GiB and a limit of 2.5GiB - and tell it to use all of it
3. Deploy the app with a request and limit of 1GiB - and tell it to use all of it
4. Observe what happens to the app that exceeds its request (but not its limit)

In this example, the original app (exceeding its request, but not limit) will be OOM killed to make way for the new app.

## Why?

This is to prove what happens if a customer sets different memory requests and limits and then ends up in a scenario where there's not enough free memory to serve its allocated limits (but can serve its requests).

Ultimately, it aims to demonstrate that you should almost never set different memory requests and limits unless your app can tolerate being OOM killed by the scheduler.

## Procedure

### Step 1 - Create a GKE cluster

Create a Kubernetes cluster with a single node and 4GB RAM.

```c
// TODO - add some cluster creation instructions here
```

### Step 2 - Uneven Deployment

The `uneven-deployment.yaml` manifest deploys an app that will consume ~2.5GiB of RAM, with a request of 1GiB and a limit of 2.5GiB.

```shell
kubectl apply -f uneven-deployment.yaml
```

The app may take a few mins to startu, so give it a bit and check the memory usage:

```shell
% kubectl top pods
NAME                           CPU(cores)   MEMORY(bytes)
uneven-test-7f487f79b6-mb42v   0m           2515Mi
```

In this case, we're using ~1.5GiB more RAM than the request, but well within the limit of 2.5GiB.

### Step 3 - Even Deployment

The `even-deployment.yaml` manifest deploys an app that will consume ~1GiB of RAM with a request of 1GiB and a limit of 1GiB.

```shell
kubectl apply -f even-deployment.yaml
```

The combination of these two apps will exceed the total available memory of the node and evict the original workload that's exceeding its request

```shell
% kubectl get pods
NAME                           READY   STATUS              RESTARTS   AGE
even-test-6c78bc9bb4-8snqj     1/1     Running             0          4s
uneven-test-7f487f79b6-mb42v   0/1     Evicted             0          2m21s
uneven-test-7f487f79b6-rq6pt   0/1     ContainerCreating   0          1s
```

Eventually the node will be tainted to forbid all scheduling:

```shell
% kubectl describe nodes | grep '^Taints'
Taints:             node.kubernetes.io/memory-pressure:NoSchedule
```
